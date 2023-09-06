#include <iostream>
#include <exception>
#include <bitset>

const int INT_BIT_SIZE = 8 * sizeof(int);

struct Packet {
        std::bitset<16> words[4];
};

void inputToFirstWord(int x, int y, std::bitset<16>& word) {
        if ((x >= 0 && x <= 63) && (y >= -32 && y <= 31)) {
                std::bitset<16> xBit = x & 0b0000'0000'0011'1111;
                std::bitset<16> yBit = (y << 8) & 0b0011'1111'0000'0000;

                word = yBit | xBit;
        }
        else {
                throw std::runtime_error("inputToFirstWord");
        }
}

void inputToSecondWord(int v, int m, int s, std::bitset<16>& word) {
        if ((v >= 0 && v <= 255) && (m >= 0 && m <= 3) && (s >= 0 && s <= 3)) {
                std::bitset<16> vBit = v & 0b0000'0000'1111'1111;
                std::bitset<16> mBit = (m << 8) & 0b0000'0011'0000'0000;
                std::bitset<16> sBit = (s << 12) & 0b0011'0000'0000'0000;

                word = sBit | mBit | vBit;
        }
        else {
                throw std::runtime_error("inputToSecondWord");
        }
}

void inputToThirdWord(float a, int p, std::bitset<16>& word) {
        if ((a >= -12.8f && a <= 12.7f) && (p >= 0 && p <= 130)) {      // -12.7 until 12.8 by the task
                int8_t intNumA = static_cast<int8_t>(a * 10.0f); // static_cast<int>(a * 10.0f - 1.0f);

                std::bitset<16> aBit = intNumA & 0b0000'0000'1111'1111;
                std::bitset<16> pBit = (p << 8) & 0b1111'1111'0000'0000;

                word = pBit | aBit;
        }
        else {
                throw std::runtime_error("inputToThirdWord");
        }
}

void inputToFourthWord(int r, std::bitset<16>& word) {
        if (r >= 0 && r <= 0b1111'1111'1111'1111) {
                word = r;
        }
        else {
                throw std::runtime_error("inputToFourthWord");
        }
}

void outputFromFirstWord(int& x, int& y, std::bitset<16> word) {
        std::bitset<INT_BIT_SIZE> xBit;
        std::bitset<INT_BIT_SIZE> yBit;

        if (word[13] == 1) yBit.set();

        for (size_t i = 0, j = 0; i <= 5; ++i, ++j) {
                xBit[j] = word[i];
        }

        for (size_t i = 8, j = 0; i <= 13; ++i, ++j) {
                yBit[j] = word[i];
        }
        
        x = static_cast<int>(std::stoll(xBit.to_string(), 0, 2));
        y = static_cast<int>(std::stoll(yBit.to_string(), 0, 2));
}

int main() {
        Packet packet;

        try {
                int x, y;

                inputToFirstWord(31, -32, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                outputFromFirstWord(x, y, packet.words[0]);
                std::cout << "x = " << x << ", y = " << y << "\n";

                inputToFirstWord(63, 31, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                outputFromFirstWord(x, y, packet.words[0]);
                std::cout << "x = " << x << ", y = " << y << "\n";

                inputToFirstWord(0, 0, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";

                outputFromFirstWord(x, y, packet.words[0]);
                std::cout << "x = " << x << ", y = " << y << "\n";

                inputToFirstWord(0, 32, packet.words[0]);
                std::cout << "words[0] = " << packet.words[0] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                inputToSecondWord(255, 3, 3, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                inputToSecondWord(0, 0, 0, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                inputToSecondWord(1, 1, 1, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";

                inputToSecondWord(1, 4, 1, packet.words[1]);
                std::cout << "words[1] = " << packet.words[1] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                inputToThirdWord(1, 1, packet.words[2]);
                std::cout << "1 words[2] = " << packet.words[2] << "\n";

                inputToThirdWord(-12.8, 0, packet.words[2]);
                std::cout << "2 words[2] = " << packet.words[2] << "\n";

                inputToThirdWord(12.7, 0, packet.words[2]);
                std::cout << "3 words[2] = " << packet.words[2] << "\n";

                inputToThirdWord(0, 121, packet.words[2]);
                std::cout << "4 words[2] = " << packet.words[2] << "\n";

                inputToThirdWord(0, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";

                inputToThirdWord(12.8, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n";

        try {
                inputToFourthWord(1, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                inputToFourthWord(0b1011'1101'1101'1011, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                inputToFourthWord(0b1111'1111'1111'1111, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                inputToFourthWord(0b0, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";

                inputToFourthWord(0b11111'1111'1111'1111, packet.words[3]);
                std::cout << "words[3] = " << packet.words[3] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }
}