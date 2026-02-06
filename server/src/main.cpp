#include "unitylink.hpp"

#include <arpa/inet.h>
#include <csignal>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace {

constexpr uint16_t kHelloReq = 0x0001;
constexpr uint16_t kWelcomeResp = 0x0002;
constexpr int kListenPort = 6464;

bool ReadExact(int fd, uint8_t* buffer, size_t len) {
  size_t offset = 0;
  while (offset < len) {
    ssize_t rc = recv(fd, buffer + offset, len - offset, 0);
    if (rc == 0) {
      return false;
    }
    if (rc < 0) {
      if (errno == EINTR) {
        continue;
      }
      return false;
    }
    offset += static_cast<size_t>(rc);
  }
  return true;
}

bool WriteAll(int fd, const uint8_t* buffer, size_t len) {
  size_t offset = 0;
  while (offset < len) {
    ssize_t rc = send(fd, buffer + offset, len - offset, 0);
    if (rc <= 0) {
      if (rc < 0 && errno == EINTR) {
        continue;
      }
      return false;
    }
    offset += static_cast<size_t>(rc);
  }
  return true;
}

} // namespace

int main() {
  std::signal(SIGPIPE, SIG_IGN);

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket: " << std::strerror(errno) << "\n";
    return 1;
  }

  int enable = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
    std::cerr << "Failed to set SO_REUSEADDR: " << std::strerror(errno) << "\n";
    close(server_fd);
    return 1;
  }

  sockaddr_in addr;
  std::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(static_cast<uint16_t>(kListenPort));

  if (bind(server_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
    std::cerr << "Failed to bind: " << std::strerror(errno) << "\n";
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, 1) < 0) {
    std::cerr << "Failed to listen: " << std::strerror(errno) << "\n";
    close(server_fd);
    return 1;
  }

  std::cout << "UnityLobby server listening on 0.0.0.0:" << kListenPort << "\n";

  while (true) {
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_len);
    if (client_fd < 0) {
      if (errno == EINTR) {
        continue;
      }
      std::cerr << "Accept failed: " << std::strerror(errno) << "\n";
      continue;
    }

    std::cout << "Client connected\n";

    while (true) {
      uint8_t length_bytes[4];
      if (!ReadExact(client_fd, length_bytes, sizeof(length_bytes))) {
        break;
      }

      const uint32_t length = unitylink::ReadU32LE(length_bytes);
      if (length > unitylink::kMaxFrameLength) {
        std::cerr << "Frame length exceeds cap, closing connection\n";
        break;
      }

      std::vector<uint8_t> body(length);
      if (length > 0 && !ReadExact(client_fd, body.data(), body.size())) {
        break;
      }

      unitylink::Frame frame;
      std::string error;
      if (!unitylink::ParseFrameBody(body.data(), body.size(), &frame, &error)) {
        std::cerr << "Frame parse error: " << error << "\n";
        break;
      }

      const bool is_response = (frame.header.flags & unitylink::kFlagResponse) != 0;
      if (!is_response && frame.header.type == kHelloReq) {
        std::vector<uint8_t> payload = unitylink::BuildWelcomePayload();
        std::string encode_error;
        std::vector<uint8_t> response = unitylink::EncodeResponse(
            kWelcomeResp, frame.header.request_id, 0, payload, &encode_error);
        if (response.empty()) {
          std::cerr << "Failed to encode response: " << encode_error << "\n";
          break;
        }
        if (!WriteAll(client_fd, response.data(), response.size())) {
          std::cerr << "Failed to send response\n";
          break;
        }
      }
    }

    close(client_fd);
    std::cout << "Client disconnected\n";
  }

  close(server_fd);
  return 0;
}
