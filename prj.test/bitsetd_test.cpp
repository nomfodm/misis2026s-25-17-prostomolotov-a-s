#include "bitsetd.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "../prj.thirdparty/doctest/doctest.h"

TEST_CASE("[bitsetd] - ctor default") {
    BitsetD b;
    CHECK(0 == b.size());
}

TEST_CASE("[bitsetd] - ctor size val") {
    BitsetD b0(8, false);
    CHECK(8 == b0.size());
    for (int32_t i = 0; i < 8; i++) CHECK(!b0.get(i));

    BitsetD b1(8, true);
    CHECK(8 == b1.size());
    for (int32_t i = 0; i < 8; i++) CHECK(b1.get(i));

    CHECK_THROWS_AS(BitsetD(0), std::invalid_argument);
    CHECK_THROWS_AS(BitsetD(-1), std::invalid_argument);
}

TEST_CASE("[bitsetd] - ctor mask") {
    BitsetD b64(0xAAAABBBBCCCCDDDDULL, 64);
    CHECK(64 == b64.size());
    CHECK(b64.get(0));   // bit 0 of lower chunk 0xCCCCDDDD = 1
    CHECK(b64.get(32));  // bit 0 of upper chunk 0xAAAABBBB = 1

    BitsetD b8(0b10100001ULL, 8);
    CHECK(8 == b8.size());
    CHECK(b8.get(0));
    CHECK(!b8.get(1));
    CHECK(b8.get(5));
    CHECK(b8.get(7));

    // trim: bits beyond size_ must be cleared
    BitsetD b33(0xFFFFFFFFFFFFFFFFULL, 33);
    CHECK(33 == b33.size());
    CHECK_THROWS_AS(b33.get(33), std::out_of_range);

    CHECK_THROWS_AS(BitsetD(0ULL, 0), std::invalid_argument);
    CHECK_THROWS_AS(BitsetD(0ULL, -1), std::invalid_argument);
}

TEST_CASE("[bitsetd] - get set contracts") {
    BitsetD b;
    CHECK_THROWS_AS(b.get(0), std::out_of_range);
    CHECK_THROWS_AS(b.set(0, true), std::out_of_range);

    BitsetD b8(8, false);
    CHECK_THROWS_AS(b8.get(8), std::out_of_range);
    CHECK_THROWS_AS(b8.set(8, true), std::out_of_range);
}

TEST_CASE("[bitsetd] - get set") {
    BitsetD b(8, false);
    b.set(0, true);
    b.set(3, true);
    b.set(7, true);
    CHECK(b.get(0));
    CHECK(!b.get(1));
    CHECK(b.get(3));
    CHECK(!b.get(4));
    CHECK(b.get(7));

    b.set(0, false);
    CHECK(!b.get(0));
}

TEST_CASE("[bitsetd] - op[]") {
    BitsetD b(0b10100001ULL, 8);
    CHECK(b[0]);
    CHECK(!b[1]);
    CHECK(b[5]);
    CHECK(b[7]);

    b[0] = false;
    CHECK(!b[0]);
    b[2] = true;
    CHECK(b[2]);
    b[2] = b[7];  // BitW = BitW
    CHECK(b[2]);
}

TEST_CASE("[bitsetd] - op[] const") {
    const BitsetD b(0b10100001ULL, 8);
    CHECK(b[0]);
    CHECK(!b[1]);
    CHECK(b[5]);
    CHECK(b[7]);

    uint64_t mask = 1ULL;
    const auto bits = 0b0101'1010'1111'0000ULL;
    const BitsetD b64(bits, 64);
    for (int32_t i = 0; i < 64; i++) {
        CHECK(b64[i] == bool(bits & mask));
        mask <<= 1;
    }
}

TEST_CASE("[bitsetd] - fill") {
    BitsetD b(10, false);
    b.fill(true);
    for (int32_t i = 0; i < 10; i++) CHECK(b.get(i));

    b.fill(false);
    for (int32_t i = 0; i < 10; i++) CHECK(!b.get(i));
}

TEST_CASE("[bitsetd] - invert") {
    BitsetD b(8, false);
    b.set(0, true);
    b.set(3, true);
    b.invert();
    for (int32_t i = 0; i < 8; i++) {
        if (i == 0 || i == 3) CHECK(!b.get(i));
        else CHECK(b.get(i));
    }

    // double invert is identity
    const BitsetD orig(0b10100001ULL, 8);
    BitsetD a(orig);
    a.invert().invert();
    CHECK(a == orig);
}

TEST_CASE("[bitsetd] - resize") {
    BitsetD b(4, false);
    b.set(2, true);

    b.resize(8, false);
    CHECK(8 == b.size());
    CHECK(b.get(2));
    CHECK(!b.get(4));

    b.resize(2);
    CHECK(2 == b.size());
    CHECK_THROWS_AS(b.get(2), std::out_of_range);

    CHECK_THROWS_AS(b.resize(0), std::invalid_argument);
    CHECK_THROWS_AS(b.resize(-1), std::invalid_argument);
}

TEST_CASE("[bitsetd] - shift") {
    // cyclic left shift: bit i -> (i + s) % size
    BitsetD b(8, false);
    b.set(1, true);
    b.set(4, true);
    b.shift(2);
    CHECK(b.get(3));
    CHECK(b.get(6));
    CHECK(!b.get(1));
    CHECK(!b.get(4));

    // shift by multiple of size = identity
    BitsetD a(0b10100001ULL, 8);
    const BitsetD copy(a);
    a.shift(8);
    CHECK(a == copy);

    // wrap around
    BitsetD c(8, false);
    c.set(6, true);
    c.shift(3);
    CHECK(c.get(1));
    CHECK(!c.get(6));
}

TEST_CASE("[bitsetd] - op==") {
    BitsetD a(8, false);
    BitsetD b(8, false);
    CHECK(a == b);

    a.set(3, true);
    CHECK(!(a == b));

    b.set(3, true);
    CHECK(a == b);

    BitsetD c(4, false);
    CHECK(!(a == c));  // different size
}

TEST_CASE("[bitsetd] - op<<=") {
    BitsetD b(8, false);
    b.set(0, true);
    b.set(2, true);
    b <<= 3;
    CHECK(b.get(3));
    CHECK(b.get(5));
    CHECK(!b.get(0));
    CHECK(!b.get(2));

    // shift >= size fills with false
    BitsetD a(8, true);
    a <<= 8;
    for (int32_t i = 0; i < 8; i++) CHECK(!a.get(i));
}

TEST_CASE("[bitsetd] - op>>=") {
    BitsetD b(8, false);
    b.set(3, true);
    b.set(5, true);
    b >>= 3;
    CHECK(b.get(0));
    CHECK(b.get(2));
    CHECK(!b.get(3));
    CHECK(!b.get(5));

    // shift >= size fills with false
    BitsetD a(8, true);
    a >>= 8;
    for (int32_t i = 0; i < 8; i++) CHECK(!a.get(i));
}

TEST_CASE("[bitsetd] - op&=") {
    BitsetD a(8, false), b(8, false);
    a.set(0, true); a.set(3, true);
    b.set(0, true); b.set(5, true);
    a &= b;
    CHECK(a.get(0));
    CHECK(!a.get(3));
    CHECK(!a.get(5));

    BitsetD c(4, false);
    CHECK_THROWS_AS(a &= c, std::invalid_argument);
}

TEST_CASE("[bitsetd] - op|=") {
    BitsetD a(8, false), b(8, false);
    a.set(0, true); a.set(3, true);
    b.set(0, true); b.set(5, true);
    a |= b;
    CHECK(a.get(0));
    CHECK(a.get(3));
    CHECK(a.get(5));
    CHECK(!a.get(1));

    BitsetD c(4, false);
    CHECK_THROWS_AS(a |= c, std::invalid_argument);
}

TEST_CASE("[bitsetd] - op^=") {
    BitsetD a(8, false), b(8, false);
    a.set(0, true); a.set(3, true);
    b.set(0, true); b.set(5, true);
    a ^= b;
    CHECK(!a.get(0));
    CHECK(a.get(3));
    CHECK(a.get(5));

    BitsetD c(4, false);
    CHECK_THROWS_AS(a ^= c, std::invalid_argument);
}

TEST_CASE("[bitsetd] - op~") {
    BitsetD b(8, false);
    b.set(0, true);
    const BitsetD nb = ~b;
    CHECK(!nb.get(0));
    for (int32_t i = 1; i < 8; i++) CHECK(nb.get(i));
    CHECK(b.get(0));  // original unchanged

    // ~~a == a
    const BitsetD a(0b10100001ULL, 8);
    CHECK(~~a == a);
}

TEST_CASE("[bitsetd] - op<< op>>") {
    const BitsetD shifted_l = BitsetD(0b00000001ULL, 8) << 4;
    CHECK(shifted_l.get(4));
    CHECK(!shifted_l.get(0));

    const BitsetD shifted_r = BitsetD(0b00010000ULL, 8) >> 4;
    CHECK(shifted_r.get(0));
    CHECK(!shifted_r.get(4));

    // originals unchanged
    BitsetD orig(8, false);
    orig.set(0, true);
    const BitsetD r = orig << 4;
    CHECK(!orig.get(4));
    CHECK(!r.get(0));
}

TEST_CASE("[bitsetd] - op& op| op^") {
    const BitsetD a(0b10101010ULL, 8);
    const BitsetD b(0b11001100ULL, 8);

    CHECK((a & b) == BitsetD(0b10001000ULL, 8));
    CHECK((a | b) == BitsetD(0b11101110ULL, 8));
    CHECK((a ^ b) == BitsetD(0b01100110ULL, 8));
}

TEST_CASE("[bitsetd] - to_string empty") {
    BitsetD b;
    CHECK("empty" == b.to_string());
    CHECK("empty" == b.to_string(BitsetD::StrFormat::Hex));
}

TEST_CASE("[bitsetd] - to_string Bin") {
    // 0b10100001: b1010'0001
    CHECK("b1010'0001" == BitsetD(0b10100001ULL, 8).to_string(BitsetD::StrFormat::Bin));
    CHECK("b0001"      == BitsetD(1ULL, 4).to_string(BitsetD::StrFormat::Bin));
    // len pads with leading zeros
    CHECK("b0000'0001" == BitsetD(1ULL, 4).to_string(BitsetD::StrFormat::Bin, 8));
}

TEST_CASE("[bitsetd] - to_string BinNoPreSep") {
    CHECK("10100001" == BitsetD(0b10100001ULL, 8).to_string(BitsetD::StrFormat::BinNoPreSep));
    CHECK("00000001" == BitsetD(1ULL, 8).to_string(BitsetD::StrFormat::BinNoPreSep));
}

TEST_CASE("[bitsetd] - to_string Oct") {
    // 0b10100001 = 161 = 0o241
    CHECK("241" == BitsetD(0b10100001ULL, 8).to_string(BitsetD::StrFormat::Oct));
    CHECK("001" == BitsetD(1ULL, 8).to_string(BitsetD::StrFormat::Oct));
}

TEST_CASE("[bitsetd] - to_string Hex") {
    // 0b10100001 = 0xa1
    CHECK("a1" == BitsetD(0b10100001ULL, 8).to_string(BitsetD::StrFormat::Hex));
    CHECK("ff" == BitsetD(0xFFULL, 8).to_string(BitsetD::StrFormat::Hex));
    CHECK("00" == BitsetD(8, false).to_string(BitsetD::StrFormat::Hex));
}
