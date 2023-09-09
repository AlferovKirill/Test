#pragma once

#include <iostream>
#include <exception>
#include <bitset>

namespace BM {
    const int INT_BIT_SIZE = 8 * sizeof(int);

    struct Packet {
        char name = 0xAA;
        std::bitset<16> words[4];
    };

    class BitManipulator {
    public:
        Packet getPacket();
        void insertPacket(Packet _packet);

        void numsToBitsFirstWord(int x, int y);
        void numsToBitsSecondWord(int v, int m, int s);
        void numsToBitsThirdWord(float a, int p);
        void numsToBitsFourthWord(int r);

        void bitsToNumsFirstWord(int& x, int& y);
        void bitsToNumsSecondWord(int& v, int& m, int& s);
        void bitsToNumsThirdWord(float& a, int& p);
        void bitsToNumsFourthWord(int& r);
    private:
        Packet packet;
    };
}
