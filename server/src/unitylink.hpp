#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace unitylink {

constexpr uint32_t kProtoVersion = 0x00000002;
constexpr uint16_t kFlagResponse = 0x0001;
constexpr uint32_t kMaxFrameLength = 1024 * 1024; // 1 MiB cap
constexpr size_t kFrameHeaderSize = 16; // type(2) + flags(2) + request_id(4) + status(4) + proto_version(4)

struct FrameHeader {
  uint16_t type = 0;
  uint16_t flags = 0;
  uint32_t request_id = 0;
  uint32_t status = 0;
  uint32_t proto_version = 0;
};

struct Frame {
  FrameHeader header;
  std::vector<uint8_t> payload;
};

uint16_t ReadU16LE(const uint8_t* data);
uint32_t ReadU32LE(const uint8_t* data);
uint64_t ReadU64LE(const uint8_t* data);

void WriteU16LE(std::vector<uint8_t>& out, uint16_t value);
void WriteU32LE(std::vector<uint8_t>& out, uint32_t value);
void WriteU64LE(std::vector<uint8_t>& out, uint64_t value);
void WriteBytes(std::vector<uint8_t>& out, const uint8_t* data, size_t len);

bool ParseFrameBody(const uint8_t* body, size_t body_len, Frame* out, std::string* error);
bool ParseFrameBytes(const std::vector<uint8_t>& frame_bytes, Frame* out, std::string* error);

std::vector<uint8_t> EncodeResponse(uint16_t type,
                                   uint32_t request_id,
                                   uint32_t status,
                                   const std::vector<uint8_t>& payload,
                                   std::string* error);

std::vector<uint8_t> BuildWelcomePayload();

} // namespace unitylink
