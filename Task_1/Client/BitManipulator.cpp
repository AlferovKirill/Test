#include "BitManipulator.h"

BM::Packet BM::BitManipulator::getPacket() {
    return packet;
}

void BM::BitManipulator::insertPacket(Packet _packet) {
    packet = _packet;
}

void BM::BitManipulator::setFirstWord(int x, int y) {
    packet.firstWord.x = x;
    packet.firstWord.y = y;
}

void BM::BitManipulator::getFirstWord(int& x, int& y) {
    x = packet.firstWord.x;
    y = packet.firstWord.y;
}

void BM::BitManipulator::setSecondWord(int v, int m, int s) {
    packet.secondWord.v = v;
    packet.secondWord.m = m;
    packet.secondWord.s = s;
}

void BM::BitManipulator::setThirdWord(float a, int p) {
    packet.thirdWord.a = static_cast<int>(a * 10.0f);
    packet.thirdWord.p = p;
}

void BM::BitManipulator::setFourthWord(int r) {
    packet.fourthWord.r1 = (r >> 0) & 1;
    packet.fourthWord.r2 = (r >> 1) & 1;
    packet.fourthWord.r3 = (r >> 2) & 1;
    packet.fourthWord.r4 = (r >> 3) & 1;

    packet.fourthWord.r5 = (r >> 4) & 1;
    packet.fourthWord.r6 = (r >> 5) & 1;
    packet.fourthWord.r7 = (r >> 6) & 1;
    packet.fourthWord.r8 = (r >> 7) & 1;

    packet.fourthWord.r9 = (r >> 8) & 1;
    packet.fourthWord.r10 = (r >> 9) & 1;
    packet.fourthWord.r11 = (r >> 10) & 1;
    packet.fourthWord.r12 = (r >> 11) & 1;

    packet.fourthWord.r13 = (r >> 12) & 1;
    packet.fourthWord.r14 = (r >> 13) & 1;
    packet.fourthWord.r15 = (r >> 14) & 1;
    packet.fourthWord.r16 = (r >> 15) & 1;
}

void BM::BitManipulator::getSecondWord(int& v, int& m, int& s) {
    v = packet.secondWord.v;
    m = packet.secondWord.m;
    s = packet.secondWord.s;
}

void BM::BitManipulator::getThirdWord(float& a, int& p) {
    a = static_cast<float>(packet.thirdWord.a / 10.0f);
    p = packet.thirdWord.p;
}

void BM::BitManipulator::getFourthWord(int& r) {
    r = 0;

    r |= (packet.fourthWord.r1 << 0);
    r |= (packet.fourthWord.r2 << 1);
    r |= (packet.fourthWord.r3 << 2);
    r |= (packet.fourthWord.r4 << 3);

    r |= (packet.fourthWord.r5 << 4);
    r |= (packet.fourthWord.r6 << 5);
    r |= (packet.fourthWord.r7 << 6);
    r |= (packet.fourthWord.r8 << 7);

    r |= (packet.fourthWord.r9 << 8);
    r |= (packet.fourthWord.r10 << 9);
    r |= packet.fourthWord.r11 << 10;
    r |= packet.fourthWord.r12 << 11;

    r |= packet.fourthWord.r13 << 12;
    r |= packet.fourthWord.r14 << 13;
    r |= packet.fourthWord.r15 << 14;
    r |= packet.fourthWord.r16 << 15;
}
