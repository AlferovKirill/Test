#pragma once

namespace BM {
    struct FirstWord {
        unsigned char x:6;
        unsigned char space1:2;
        signed char y:6;
        unsigned char space2:2;
    };

    struct SecondWord {
        unsigned char v:8;
        unsigned char m:2;
        unsigned char space1:2;
        unsigned char s:2;
        unsigned char space2:2;
    };

    struct ThirdWord {
        signed char a:8;
        unsigned char p:8;
    };

    struct FourthWord {
        unsigned char r1:1;
        unsigned char r2:1;
        unsigned char r3:1;
        unsigned char r4:1;

        unsigned char r5:1;
        unsigned char r6:1;
        unsigned char r7:1;
        unsigned char r8:1;

        unsigned char r9:1;
        unsigned char r10:1;
        unsigned char r11:1;
        unsigned char r12:1;

        unsigned char r13:1;
        unsigned char r14:1;
        unsigned char r15:1;
        unsigned char r16:1;
    };

    struct Packet {
        FirstWord firstWord;
        SecondWord secondWord;
        ThirdWord thirdWord;
        FourthWord fourthWord;
    };

    class BitManipulator {
    public:
        Packet getPacket();
        void insertPacket(Packet _packet);

        void setFirstWord(int x, int y);
        void setSecondWord(int v, int m, int s);
        void setThirdWord(float a, int p);
        void setFourthWord(int r);

        void getFirstWord(int& x, int& y);
        void getSecondWord(int& v, int& m, int& s);
        void getThirdWord(float& a, int& p);
        void getFourthWord(int& r);
    private:
        Packet packet;
    };
}
