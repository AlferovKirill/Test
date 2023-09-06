#include <iostream>
#include <exception>
#include <bitset>

struct Packet {
        std::bitset<16> words[4];
};

void userInputToFirstWord(int x, int y, std::bitset<16>& word) {
        word = 0;

        if ((x >= 0 && x <= 63) && (y >= -32 && y <= 31)) {
                std::bitset<16> xBit = x & 0b0000'0000'0011'1111;
                std::bitset<16> yBit = (y << 8) & 0b0011'1111'0000'0000;

                word = yBit | xBit;
        }
        else {
                throw std::runtime_error("userInputToFirstWord");
        }
}

void userInputToSecondWord(int v, int m, int s, std::bitset<16>& word) {
        word = 0;

        if ((v >= 0 && v <= 255) && (m >= 0 && m <= 3) && (s >= 0 && s <= 3)) {
                std::bitset<16> vBit = v & 0b0000'0000'1111'1111;
                std::bitset<16> mBit = (m << 8) & 0b0000'0011'0000'0000;
                std::bitset<16> sBit = (s << 12) & 0b0011'0000'0000'0000;

                word = sBit | mBit | vBit;
        }
        else {
                throw std::runtime_error("userInputToSecondWord");
        }
}

void userInputToThirdWord(float a, int p, std::bitset<16>& word) {
        word = 0;

        if ((a >= -12.8f && a <= 12.7f) && (p >= 0 && p <= 130)) {      // -12.8 until 12.7
                int8_t intNumA = static_cast<int8_t>(a * 10.0f); // static_cast<int>(a * 10.0f);

                std::bitset<16> aBit = intNumA & 0b0000'0000'1111'1111;
                std::bitset<16> pBit = (p << 8) & 0b1111'1111'0000'0000;

                word = pBit | aBit;
        }
        else {
                throw std::runtime_error("userInputToThirdWord");
        }
}

void userInputToFourthWord(int r, std::bitset<16>& word) {
        word = 0;

        if (r >= 0 && r <= 0b1111'1111'1111'1111) {
                word = r;
        }
        else {
                throw std::runtime_error("userInputToFourthWord");
        }
}

int main() {
        Packet packet;

        try {
                userInputToFirstWord(31, -32, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                userInputToFirstWord(63, 31, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                userInputToFirstWord(0, 0, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                userInputToFirstWord(0, 32, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                userInputToSecondWord(255, 3, 3, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                userInputToSecondWord(0, 0, 0, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                userInputToSecondWord(1, 1, 1, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                userInputToSecondWord(1, 4, 1, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                userInputToThirdWord(1, 1, packet.words[2]);
                std::cout << "1 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(-12.7, 0, packet.words[2]);
                std::cout << "2 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(12.8, 0, packet.words[2]);
                std::cout << "3 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(0, 121, packet.words[2]);
                std::cout << "4 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(0, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(-12.8, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                userInputToFourthWord(1, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                userInputToFourthWord(0b1011'1101'1101'1011, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                userInputToFourthWord(0b1111'1111'1111'1111, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                userInputToFourthWord(0b0, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                userInputToFourthWord(0b11111'1111'1111'1111, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }
}