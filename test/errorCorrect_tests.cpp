#include <catch2/catch_test_macros.hpp>

#include <array>

#include "../include/flarm/errorCorrect.hpp"

namespace
{
    constexpr size_t PacketBytes = 26;
    constexpr size_t PayloadBytes = 24;

    std::array<uint8_t, PacketBytes> makeValidPacket()
    {
        std::array<uint8_t, PacketBytes> packet{
            0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
            0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
            0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xF1, 0x02};

        const uint16_t checksum = FLARM::flarmCalculateChecksum(
            etl::span<const uint8_t>(packet.data(), PayloadBytes),
            static_cast<uint8_t>(PayloadBytes));
        FLARM::writeChecksum(etl::span<uint8_t>(packet.data(), packet.size()), checksum);
        return packet;
    }

    etl::span<uint8_t> asMutableBytes(std::array<uint8_t, PacketBytes> &packet)
    {
        return etl::span<uint8_t>(packet.data(), packet.size());
    }

    etl::span<const uint8_t> asConstBytes(const std::array<uint8_t, PacketBytes> &packet)
    {
        return etl::span<const uint8_t>(packet.data(), packet.size());
    }
}

TEST_CASE("Correct returns 0 for an already valid FLARM packet", "[errorCorrect]")
{
    auto packet = makeValidPacket();
    std::array<uint8_t, PacketBytes> err{};

    REQUIRE(FLARM::Correct(asMutableBytes(packet), asConstBytes(err)) == 0);
    REQUIRE(packet == makeValidPacket());
}

TEST_CASE("Correct repairs two flipped bits in a FLARM packet", "[errorCorrect]")
{
    auto packet = makeValidPacket();
    std::array<uint8_t, PacketBytes> err{};

    FLARM::FlipBit(asMutableBytes(packet), 200);
    FLARM::FlipBit(asMutableBytes(err), 200);

    FLARM::FlipBit(asMutableBytes(packet), 13);
    FLARM::FlipBit(asMutableBytes(err), 13);

    REQUIRE(FLARM::Correct(asMutableBytes(packet), asConstBytes(err)) == 2);
    REQUIRE(packet == makeValidPacket());
}
