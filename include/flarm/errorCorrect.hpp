#pragma once

#include <cstddef>
#include <cstdint>

#include <etl/array.h>
#include <etl/span.h>

#include "lib_crc.hpp"

namespace FLARM
{
  constexpr size_t MAX_PACKET_BYTES = 26;
  constexpr size_t MAX_PACKET_BITS = MAX_PACKET_BYTES * 8;

  static constexpr uint16_t SyndromeBits[208] = {
      0x22DA, 0x116D, 0x80A6, 0x4053, 0xA839, 0xDC0C, 0x6E06, 0x3703,
      0x9391, 0xC1D8, 0x60EC, 0x3076, 0x183B, 0x840D, 0xCA16, 0x650B,
      0xBA95, 0xD55A, 0x6AAD, 0xBD46, 0x5EA3, 0xA741, 0xDBB0, 0x6DD8,
      0x36EC, 0x1B76, 0x0DBB, 0x8ECD, 0xCF76, 0x67BB, 0xBBCD, 0xD5F6,
      0x6AFB, 0xBD6D, 0xD6A6, 0x6B53, 0xBDB9, 0xD6CC, 0x6B66, 0x35B3,
      0x92C9, 0xC174, 0x60BA, 0x305D, 0x903E, 0x481F, 0xAC1F, 0xDE1F,
      0xE71F, 0xFB9F, 0xF5DF, 0xF2FF, 0xF16F, 0xF0A7, 0xF043, 0xF031,
      0xF008, 0x7804, 0x3C02, 0x1E01, 0x8710, 0x4388, 0x21C4, 0x10E2,
      0x0871, 0x8C28, 0x4614, 0x230A, 0x1185, 0x80D2, 0x4069, 0xA824,
      0x5412, 0x2A09, 0x9D14, 0x4E8A, 0x2745, 0x9BB2, 0x4DD9, 0xAEFC,
      0x577E, 0x2BBF, 0x9DCF, 0xC6F7, 0xEB6B, 0xFDA5, 0xF6C2, 0x7B61,
      0xB5A0, 0x5AD0, 0x2D68, 0x16B4, 0x0B5A, 0x05AD, 0x8AC6, 0x4563,
      0xAAA1, 0xDD40, 0x6EA0, 0x3750, 0x1BA8, 0x0DD4, 0x06EA, 0x0375,
      0x89AA, 0x44D5, 0xAA7A, 0x553D, 0xA28E, 0x5147, 0xA0B3, 0xD849,
      0xE434, 0x721A, 0x390D, 0x9496, 0x4A4B, 0xAD35, 0xDE8A, 0x6F45,
      0xBFB2, 0x5FD9, 0xA7FC, 0x53FE, 0x29FF, 0x9CEF, 0xC667, 0xEB23,
      0xFD81, 0xF6D0, 0x7B68, 0x3DB4, 0x1EDA, 0x0F6D, 0x8FA6, 0x47D3,
      0xABF9, 0xDDEC, 0x6EF6, 0x377B, 0x93AD, 0xC1C6, 0x60E3, 0xB861,
      0xD420, 0x6A10, 0x3508, 0x1A84, 0x0D42, 0x06A1, 0x8B40, 0x45A0,
      0x22D0, 0x1168, 0x08B4, 0x045A, 0x022D, 0x8906, 0x4483, 0xAA51,
      0xDD38, 0x6E9C, 0x374E, 0x1BA7, 0x85C3, 0xCAF1, 0xED68, 0x76B4,
      0x3B5A, 0x1DAD, 0x86C6, 0x4363, 0xA9A1, 0xDCC0, 0x6E60, 0x3730,
      0x1B98, 0x0DCC, 0x06E6, 0x0373, 0x89A9, 0xCCC4, 0x6662, 0x3331,
      0x9188, 0x48C4, 0x2462, 0x1231, 0x8108, 0x4084, 0x2042, 0x1021,
      0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100,
      0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001};

  static constexpr uint32_t FindSyndrome[208] = {
      0x22DA00, 0x116D01, 0x80A602, 0x405303, 0xA83904, 0xDC0C05, 0x6E0606, 0x370307,
      0x939108, 0xC1D809, 0x60EC0A, 0x30760B, 0x183B0C, 0x840D0D, 0xCA160E, 0x650B0F,
      0xBA9510, 0xD55A11, 0x6AAD12, 0xBD4613, 0x5EA314, 0xA74115, 0xDBB016, 0x6DD817,
      0x36EC18, 0x1B7619, 0x0DBB1A, 0x8ECD1B, 0xCF761C, 0x67BB1D, 0xBBCD1E, 0xD5F61F,
      0x6AFB20, 0xBD6D21, 0xD6A622, 0x6B5323, 0xBDB924, 0xD6CC25, 0x6B6626, 0x35B327,
      0x92C928, 0xC17429, 0x60BA2A, 0x305D2B, 0x903E2C, 0x481F2D, 0xAC1F2E, 0xDE1F2F,
      0xE71F30, 0xFB9F31, 0xF5DF32, 0xF2FF33, 0xF16F34, 0xF0A735, 0xF04336, 0xF03137,
      0xF00838, 0x780439, 0x3C023A, 0x1E013B, 0x87103C, 0x43883D, 0x21C43E, 0x10E23F,
      0x087140, 0x8C2841, 0x461442, 0x230A43, 0x118544, 0x80D245, 0x406946, 0xA82447,
      0x541248, 0x2A0949, 0x9D144A, 0x4E8A4B, 0x27454C, 0x9BB24D, 0x4DD94E, 0xAEFC4F,
      0x577E50, 0x2BBF51, 0x9DCF52, 0xC6F753, 0xEB6B54, 0xFDA555, 0xF6C256, 0x7B6157,
      0xB5A058, 0x5AD059, 0x2D685A, 0x16B45B, 0x0B5A5C, 0x05AD5D, 0x8AC65E, 0x45635F,
      0xAAA160, 0xDD4061, 0x6EA062, 0x375063, 0x1BA864, 0x0DD465, 0x06EA66, 0x037567,
      0x89AA68, 0x44D569, 0xAA7A6A, 0x553D6B, 0xA28E6C, 0x51476D, 0xA0B36E, 0xD8496F,
      0xE43470, 0x721A71, 0x390D72, 0x949673, 0x4A4B74, 0xAD3575, 0xDE8A76, 0x6F4577,
      0xBFB278, 0x5FD979, 0xA7FC7A, 0x53FE7B, 0x29FF7C, 0x9CEF7D, 0xC6677E, 0xEB237F,
      0xFD8180, 0xF6D081, 0x7B6882, 0x3DB483, 0x1EDA84, 0x0F6D85, 0x8FA686, 0x47D387,
      0xABF988, 0xDDEC89, 0x6EF68A, 0x377B8B, 0x93AD8C, 0xC1C68D, 0x60E38E, 0xB8618F,
      0xD42090, 0x6A1091, 0x350892, 0x1A8493, 0x0D4294, 0x06A195, 0x8B4096, 0x45A097,
      0x22D098, 0x116899, 0x08B49A, 0x045A9B, 0x022D9C, 0x89069D, 0x44839E, 0xAA519F,
      0xDD38A0, 0x6E9CA1, 0x374EA2, 0x1BA7A3, 0x85C3A4, 0xCAF1A5, 0xED68A6, 0x76B4A7,
      0x3B5AA8, 0x1DADA9, 0x86C6AA, 0x4363AB, 0xA9A1AC, 0xDCC0AD, 0x6E60AE, 0x3730AF,
      0x1B98B0, 0x0DCCB1, 0x06E6B2, 0x0373B3, 0x89A9B4, 0xCCC4B5, 0x6662B6, 0x3331B7,
      0x9188B8, 0x48C4B9, 0x2462BA, 0x1231BB, 0x8108BC, 0x4084BD, 0x2042BE, 0x1021BF,
      0x8000C0, 0x4000C1, 0x2000C2, 0x1000C3, 0x0800C4, 0x0400C5, 0x0200C6, 0x0100C7,
      0x0080C8, 0x0040C9, 0x0020CA, 0x0010CB, 0x0008CC, 0x0004CD, 0x0002CE, 0x0001CF};

  uint16_t flarmCalculateChecksum(etl::span<const uint8_t> flarm_pkt, uint8_t length)
  {
    uint16_t crc16 = 0xffff;
    crc16 = update_crc_ccitt(crc16, 0x31);
    crc16 = update_crc_ccitt(crc16, 0xFA);
    crc16 = update_crc_ccitt(crc16, 0xB6);

    for (uint8_t i = 0; i < length; ++i)
    {
      crc16 = update_crc_ccitt(crc16, static_cast<char>(flarm_pkt[i]));
    }

    return crc16;
  }

  uint16_t readChecksum(etl::span<const uint8_t> packet)
  {
    if (packet.size() < 2)
    {
      return 0;
    }

    const size_t checksumOffset = packet.size() - 2;
    return static_cast<uint16_t>(packet[checksumOffset] << 8) | packet[checksumOffset + 1];
  }

  void writeChecksum(etl::span<uint8_t> packet, uint16_t checksum)
  {
    if (packet.size() < 2)
    {
      return;
    }

    const size_t checksumOffset = packet.size() - 2;
    packet[checksumOffset] = static_cast<uint8_t>(checksum >> 8);
    packet[checksumOffset + 1] = static_cast<uint8_t>(checksum & 0xFF);
  }

  uint16_t packetSyndrome(etl::span<const uint8_t> packet)
  {
    if (packet.size() < 2)
    {
      return 0;
    }

    const uint16_t calculated = flarmCalculateChecksum(packet, static_cast<uint8_t>(packet.size() - 2));
    const uint16_t stored = readChecksum(packet);
    return calculated ^ stored;
  }

  inline uint16_t CRCsyndrome(size_t bit)
  {
    if (bit >= MAX_PACKET_BITS)
    {
      return 0;
    }

    return SyndromeBits[bit];
  }

  uint8_t FindCRCsyndrome(uint16_t syndr)
  {
    for (size_t i = 0; i < MAX_PACKET_BITS; ++i)
    {
      const uint32_t packed = FindSyndrome[i];
      if (static_cast<uint16_t>(packed >> 8) == syndr)
      {
        return static_cast<uint8_t>(packed & 0xFF);
      }
    }

    return 0xFF;
  }

  inline uint8_t Count1s(uint8_t byte)
  {
    return __builtin_popcount(byte);
  }

  inline uint8_t FindLowestSetBit(uint8_t val)
  {
    return __builtin_ctz(val);
  }

  inline void FlipBit(etl::span<uint8_t> byte, int bitIdx)
  {
    int byteIdx = bitIdx >> 3;
    bitIdx &= 7;
    bitIdx = 7 - bitIdx;
    uint8_t mask = 1;
    mask <<= bitIdx;
    byte[byteIdx] ^= mask;
  }

  int _Correct(etl::span<uint8_t> pktData, etl::span<const uint8_t> pktErr)
  {
    constexpr size_t MAX_BAD_BITS = 6;
    const uint32_t pktSize = pktData.size();

    if (pktSize < 2 || pktSize > MAX_PACKET_BYTES || pktErr.size() != pktSize)
    {
      return -1;
    }

    uint16_t syndrome = packetSyndrome(pktData);
    if (syndrome == 0)
    {
      return 0;
    }

    uint8_t errBit = FindCRCsyndrome(syndrome);
    if (errBit != 0xFF)
    {
      FlipBit(pktData, errBit);
      return 1;
    }

    uint8_t badBitIdx[MAX_BAD_BITS];
    uint8_t badBitMask[MAX_BAD_BITS];
    uint16_t bitSyndrome[MAX_BAD_BITS];
    uint32_t badBits = 0;

    for (uint8_t byteIdx = 0; byteIdx < pktSize; ++byteIdx)
    {
      const uint8_t byte = pktErr[byteIdx];
      if (!byte)
      {
        continue;
      }

      for (uint8_t bitIdx = 0; bitIdx < 8; ++bitIdx)
      {
        if (byte & (0x80u >> bitIdx))
        {
          if (badBits < MAX_BAD_BITS)
          {
            badBitIdx[badBits] = byteIdx;
            badBitMask[badBits] = static_cast<uint8_t>(0x80u >> bitIdx);
            bitSyndrome[badBits] = CRCsyndrome(byteIdx * 8 + bitIdx);
          }
          if (++badBits > MAX_BAD_BITS)
          {
            goto search_done;
          }
        }
      }
    }

  search_done:
    if (badBits > MAX_BAD_BITS)
    {
      return -1;
    }

    const uint32_t loops = 1u << badBits;
    uint32_t prevGrayIdx = 0;

    for (uint8_t idx = 1; idx < loops; ++idx)
    {
      const uint32_t grayIdx = idx ^ (idx >> 1);
      const uint32_t bit = FindLowestSetBit(static_cast<uint8_t>(grayIdx ^ prevGrayIdx));

      pktData[badBitIdx[bit]] ^= badBitMask[bit];
      syndrome ^= bitSyndrome[bit];

      if (syndrome == 0)
      {
        return Count1s(static_cast<uint8_t>(grayIdx));
      }

      errBit = FindCRCsyndrome(syndrome);
      if (errBit != 0xFF)
      {
        FlipBit(pktData, errBit);
        return Count1s(static_cast<uint8_t>(grayIdx)) + 1;
      }

      prevGrayIdx = grayIdx;
    }

    return -1;
  }

  int Correct(etl::span<uint8_t> pktData, etl::span<const uint8_t> pktErr)
  {
    if (pktData.size() < 2 || pktErr.size() != pktData.size())
    {
      return -1;
    }

    return _Correct(pktData, pktErr);
  }

} // namespace FLARM
