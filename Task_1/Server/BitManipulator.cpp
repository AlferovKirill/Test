#include "BitManipulator.h"

BM::Packet BM::BitManipulator::getPacket() {
    return packet;
}

void BM::BitManipulator::numsToBitsFirstWord(int x, int y) {
    if ((x >= 0 && x <= 63) && (y >= -32 && y <= 31)) {
        std::bitset<16> xBit = x & 0b0000000000111111;
        std::bitset<16> yBit = (y << 8) & 0b0011111100000000;

        packet.words[0] = yBit | xBit;
    }
    else {
        throw std::runtime_error("numsToBitsFirstWord");
    }
}

void BM::BitManipulator::numsToBitsSecondWord(int v, int m, int s) {
    if ((v >= 0 && v <= 255) && (m >= 0 && m <= 3) && (s >= 0 && s <= 3)) {
        std::bitset<16> vBit = v & 0b0000000011111111;
        std::bitset<16> mBit = (m << 8) & 0b0000001100000000;
        std::bitset<16> sBit = (s << 12) & 0b0011000000000000;

        packet.words[1] = sBit | mBit | vBit;
    }
    else {
        throw std::runtime_error("numsToBitsSecondWord");
    }
}

void BM::BitManipulator::numsToBitsThirdWord(float a, int p) {
    if ((a >= -12.8f && a <= 12.7f) && (p >= 0 && p <= 130)) {      // -12.7 until 12.8 by the task
        int8_t intNumA = static_cast<int8_t>(a * 10.0f); // static_cast<int>(a * 10.0f - 1.0f);

        std::bitset<16> aBit = intNumA & 0b0000000011111111;
        std::bitset<16> pBit = (p << 8) & 0b1111111100000000;

        packet.words[2] = pBit | aBit;
    }
    else {
        throw std::runtime_error("numsToBitsThirdWord");
    }
}

void BM::BitManipulator::numsToBitsFourthWord(int r) {
    if (r >= 0 && r <= 0b1111111111111111) {
        packet.words[3] = r;
    }
    else {
        throw std::runtime_error("numsToBitsFourthWord");
    }
}

void BM::BitManipulator::bitsToNumsFirstWord(int& x, int& y) {
    std::bitset<INT_BIT_SIZE> xBit;
    std::bitset<INT_BIT_SIZE> yBit;

    if (packet.words[0][13] == 1) yBit.set();

    for (size_t i = 0, j = 0; i <= 5; ++i, ++j) {
        xBit[j] = packet.words[0][i];
    }

    for (size_t i = 8, j = 0; i <= 13; ++i, ++j) {
        yBit[j] = packet.words[0][i];
    }

    x = static_cast<int>(std::stoll(xBit.to_string(), 0, 2));
    y = static_cast<int>(std::stoll(yBit.to_string(), 0, 2));
}

void BM::BitManipulator::bitsToNumsSecondWord(int& v, int& m, int& s) {
    std::bitset<INT_BIT_SIZE> vBit;
    std::bitset<INT_BIT_SIZE> mBit;
    std::bitset<INT_BIT_SIZE> sBit;

    for (size_t i = 0, j = 0; i <= 7; ++i, ++j) {
        vBit[j] = packet.words[1][i];
    }

    for (size_t i = 8, j = 0; i <= 9; ++i, ++j) {
        mBit[j] = packet.words[1][i];
    }

    for (size_t i = 12, j = 0; i <= 13; ++i, ++j) {
        sBit[j] = packet.words[1][i];
    }

    v = static_cast<int>(std::stoll(vBit.to_string(), 0, 2));
    m = static_cast<int>(std::stoll(mBit.to_string(), 0, 2));
    s = static_cast<int>(std::stoll(sBit.to_string(), 0, 2));
}

void BM::BitManipulator::bitsToNumsThirdWord(float& a, int& p) {
    std::bitset<INT_BIT_SIZE> aBit;
    std::bitset<INT_BIT_SIZE> pBit;

    if (packet.words[2][7] == 1) aBit.set();

    for (size_t i = 0, j = 0; i <= 7; ++i, ++j) {
        aBit[j] = packet.words[2][i];
    }

    for (size_t i = 8, j = 0; i <= 15; ++i, ++j) {
        pBit[j] = packet.words[2][i];
    }

    a = static_cast<int>(std::stoll(aBit.to_string(), 0, 2)) / 10.0f;
    p = static_cast<int>(std::stoll(pBit.to_string(), 0, 2));
}

void BM::BitManipulator::bitsToNumsFourthWord(int& r) {
    std::bitset<INT_BIT_SIZE> rBit;

    for (size_t i = 0, j = 0; i <= 15; ++i, ++j) {
        rBit[j] = packet.words[3][i];
    }

    r = static_cast<int>(std::stoll(rBit.to_string(), 0, 2));
}
