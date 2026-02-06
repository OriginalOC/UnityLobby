#include "unitylink.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

bool FindRepoRoot(std::filesystem::path* out_root) {
  std::filesystem::path current = std::filesystem::current_path();
  for (int i = 0; i < 8; ++i) {
    std::filesystem::path candidate = current / "test-vectors" / "frames";
    if (std::filesystem::exists(candidate) && std::filesystem::is_directory(candidate)) {
      if (out_root) {
        *out_root = current;
      }
      return true;
    }
    if (!current.has_parent_path()) {
      break;
    }
    current = current.parent_path();
  }
  return false;
}

int HexValue(char c) {
  if (c >= '0' && c <= '9') {
    return c - '0';
  }
  if (c >= 'a' && c <= 'f') {
    return 10 + (c - 'a');
  }
  if (c >= 'A' && c <= 'F') {
    return 10 + (c - 'A');
  }
  return -1;
}

bool DecodeHexFile(const std::filesystem::path& path,
                   std::vector<uint8_t>* out,
                   std::string* error) {
  std::ifstream input(path);
  if (!input) {
    if (error) {
      *error = "Failed to open file";
    }
    return false;
  }

  std::string hex;
  std::string line;
  while (std::getline(input, line)) {
    const size_t comment_pos = line.find('#');
    if (comment_pos != std::string::npos) {
      line = line.substr(0, comment_pos);
    }
    for (char c : line) {
      if (std::isxdigit(static_cast<unsigned char>(c)) != 0) {
        hex.push_back(c);
      }
    }
  }

  if (hex.size() % 2 != 0) {
    if (error) {
      *error = "Odd number of hex digits";
    }
    return false;
  }

  std::vector<uint8_t> bytes;
  bytes.reserve(hex.size() / 2);
  for (size_t i = 0; i < hex.size(); i += 2) {
    int hi = HexValue(hex[i]);
    int lo = HexValue(hex[i + 1]);
    if (hi < 0 || lo < 0) {
      if (error) {
        *error = "Invalid hex digit";
      }
      return false;
    }
    bytes.push_back(static_cast<uint8_t>((hi << 4) | lo));
  }

  if (out) {
    *out = std::move(bytes);
  }
  return true;
}

} // namespace

int main() {
  std::filesystem::path repo_root;
  if (!FindRepoRoot(&repo_root)) {
    std::cerr << "Unable to locate repo root containing test-vectors/frames" << "\n";
    return 1;
  }

  std::filesystem::path frames_dir = repo_root / "test-vectors" / "frames";
  std::vector<std::filesystem::path> files;
  for (const auto& entry : std::filesystem::directory_iterator(frames_dir)) {
    if (!entry.is_regular_file()) {
      continue;
    }
    if (entry.path().extension() == ".hex") {
      files.push_back(entry.path());
    }
  }
  std::sort(files.begin(), files.end());

  if (files.empty()) {
    std::cerr << "No .hex files found in " << frames_dir << "\n";
    return 1;
  }

  bool ok = true;
  for (const auto& file : files) {
    std::vector<uint8_t> bytes;
    std::string error;
    if (!DecodeHexFile(file, &bytes, &error)) {
      std::cerr << file.filename().string() << ": decode error: " << error << "\n";
      ok = false;
      continue;
    }

    if (bytes.size() < 4) {
      std::cerr << file.filename().string() << ": too short for length" << "\n";
      ok = false;
      continue;
    }

    const uint32_t length = unitylink::ReadU32LE(bytes.data());
    if (bytes.size() != static_cast<size_t>(length) + 4) {
      std::cerr << file.filename().string() << ": length mismatch" << "\n";
      ok = false;
      continue;
    }

    unitylink::Frame frame;
    if (!unitylink::ParseFrameBytes(bytes, &frame, &error)) {
      std::cerr << file.filename().string() << ": parse error: " << error << "\n";
      ok = false;
      continue;
    }

    std::cout << file.filename().string()
              << ": type=0x" << std::hex << frame.header.type
              << " flags=0x" << frame.header.flags
              << " request_id=" << std::dec << frame.header.request_id
              << " status=" << frame.header.status
              << " proto=0x" << std::hex << frame.header.proto_version
              << " payload=" << std::dec << frame.payload.size() << " bytes\n";
  }

  return ok ? 0 : 1;
}
