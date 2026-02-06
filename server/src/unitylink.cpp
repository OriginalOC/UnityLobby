#include "unitylink.hpp"

#include <algorithm>

namespace unitylink {

uint16_t ReadU16LE(const uint8_t* data) {
  return static_cast<uint16_t>(data[0]) |
         (static_cast<uint16_t>(data[1]) << 8);
}

uint32_t ReadU32LE(const uint8_t* data) {
  return static_cast<uint32_t>(data[0]) |
         (static_cast<uint32_t>(data[1]) << 8) |
         (static_cast<uint32_t>(data[2]) << 16) |
         (static_cast<uint32_t>(data[3]) << 24);
}

uint64_t ReadU64LE(const uint8_t* data) {
  return static_cast<uint64_t>(data[0]) |
         (static_cast<uint64_t>(data[1]) << 8) |
         (static_cast<uint64_t>(data[2]) << 16) |
         (static_cast<uint64_t>(data[3]) << 24) |
         (static_cast<uint64_t>(data[4]) << 32) |
         (static_cast<uint64_t>(data[5]) << 40) |
         (static_cast<uint64_t>(data[6]) << 48) |
         (static_cast<uint64_t>(data[7]) << 56);
}

void WriteU16LE(std::vector<uint8_t>& out, uint16_t value) {
  out.push_back(static_cast<uint8_t>(value & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
}

void WriteU32LE(std::vector<uint8_t>& out, uint32_t value) {
  out.push_back(static_cast<uint8_t>(value & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
}

void WriteU64LE(std::vector<uint8_t>& out, uint64_t value) {
  out.push_back(static_cast<uint8_t>(value & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 32) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 40) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 48) & 0xFF));
  out.push_back(static_cast<uint8_t>((value >> 56) & 0xFF));
}

void WriteBytes(std::vector<uint8_t>& out, const uint8_t* data, size_t len) {
  out.insert(out.end(), data, data + len);
}

bool ParseFrameBody(const uint8_t* body, size_t body_len, Frame* out, std::string* error) {
  if (body_len < kFrameHeaderSize) {
    if (error) {
      *error = "Frame body too short for header";
    }
    return false;
  }

  FrameHeader header;
  header.type = ReadU16LE(body + 0);
  header.flags = ReadU16LE(body + 2);
  header.request_id = ReadU32LE(body + 4);
  header.status = ReadU32LE(body + 8);
  header.proto_version = ReadU32LE(body + 12);

  if (header.proto_version != kProtoVersion) {
    if (error) {
      *error = "Unsupported proto_version";
    }
    return false;
  }

  const bool is_response = (header.flags & kFlagResponse) != 0;
  if (!is_response) {
    if (header.request_id == 0) {
      if (error) {
        *error = "Request has request_id=0";
      }
      return false;
    }
    if (header.status != 0) {
      if (error) {
        *error = "Request has non-zero status";
      }
      return false;
    }
  }

  if (out) {
    out->header = header;
    const size_t payload_len = body_len - kFrameHeaderSize;
    out->payload.assign(body + kFrameHeaderSize, body + kFrameHeaderSize + payload_len);
  }

  return true;
}

bool ParseFrameBytes(const std::vector<uint8_t>& frame_bytes, Frame* out, std::string* error) {
  if (frame_bytes.size() < 4) {
    if (error) {
      *error = "Frame too short for length field";
    }
    return false;
  }

  const uint32_t length = ReadU32LE(frame_bytes.data());
  if (length > kMaxFrameLength) {
    if (error) {
      *error = "Frame length exceeds cap";
    }
    return false;
  }

  if (frame_bytes.size() != static_cast<size_t>(length) + 4) {
    if (error) {
      *error = "Frame length mismatch";
    }
    return false;
  }

  return ParseFrameBody(frame_bytes.data() + 4, length, out, error);
}

std::vector<uint8_t> EncodeResponse(uint16_t type,
                                   uint32_t request_id,
                                   uint32_t status,
                                   const std::vector<uint8_t>& payload,
                                   std::string* error) {
  if (request_id == 0) {
    if (error) {
      *error = "Response request_id must be non-zero";
    }
    return {};
  }

  if (payload.size() > (kMaxFrameLength - kFrameHeaderSize)) {
    if (error) {
      *error = "Payload exceeds max frame length";
    }
    return {};
  }

  std::vector<uint8_t> body;
  body.reserve(kFrameHeaderSize + payload.size());
  WriteU16LE(body, type);
  WriteU16LE(body, kFlagResponse);
  WriteU32LE(body, request_id);
  WriteU32LE(body, status);
  WriteU32LE(body, kProtoVersion);
  if (!payload.empty()) {
    WriteBytes(body, payload.data(), payload.size());
  }

  const uint32_t length = static_cast<uint32_t>(body.size());
  std::vector<uint8_t> frame;
  frame.reserve(4 + body.size());
  WriteU32LE(frame, length);
  WriteBytes(frame, body.data(), body.size());
  return frame;
}

std::vector<uint8_t> BuildWelcomePayload() {
  std::vector<uint8_t> payload;
  payload.reserve(64);

  WriteU32LE(payload, 0); // server_version_major
  WriteU32LE(payload, 2); // server_version_minor

  const std::array<uint8_t, 16> session_id = {
      0x10, 0x32, 0x54, 0x76, 0x98, 0xBA, 0xDC, 0xFE,
      0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01};
  WriteBytes(payload, session_id.data(), session_id.size());

  WriteU64LE(payload, 0); // server_capabilities
  WriteU32LE(payload, 16384); // recommended_frame_payload
  WriteU32LE(payload, 16); // max_inflight_requests
  WriteU32LE(payload, kMaxFrameLength); // max_frame_length_bytes

  const std::string message = "welcome";
  WriteU32LE(payload, static_cast<uint32_t>(message.size()));
  WriteBytes(payload, reinterpret_cast<const uint8_t*>(message.data()), message.size());

  WriteU16LE(payload, 0); // TLVBlock u16(0)

  return payload;
}

} // namespace unitylink
