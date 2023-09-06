#include <iostream>
#include <exception>
#include <bitset>

struct Packet {
        std::bitset<16> words[4];
};

void userInputToFirstWord(int x, int y, std::bitset<16>& word) {
        word = 0;

        if ((x >= 0 && x <= 63) && (y >= -32 && y <= 31)) {
                word = (~((1<<15)|(1<<14))) & (y << 8 | x);
        }
        else {
                throw std::runtime_error("userInputToFirstWord");
        }
}

void userInputToSecondWord(int v, int m, int s, std::bitset<16>& word) {
        word = 0;

        if ((v >= 0 && v <= 255) && (m >= 0 && m <= 3) && (s >= 0 && s <= 3)) {
                word = (~((1<<15)|(1<<14) | (1<<11) | (1<<10))) & (s << 12 | m << 8 | v);
        }
        else {
                throw std::runtime_error("userInputToSecondWord");
        }
}

void userInputToThirdWord(double a, int p, std::bitset<16>& word) {
        word = 0;

        if ((a >= -12.7 && a <= 12.8) && (p >= 0 && p <= 130)) {
                int t = (a * 10) - 1;
                //int8_t transform = static_cast<int8_t>((a * 10) - 1);
                word = t;//(p << 8) | transform;
        }
        else {
                throw std::runtime_error("userInputToThirdWord");
        }
}

void userInputToFourthWord(int r, std::bitset<16>& word) {
        word = 0;

        if (r >= 0b0 && r <= 0b1111'1111'1111'1111) {
                word = r;
        }
        else {
                throw std::runtime_error("userInputToFourthWord");
        }
}

int main() {
        Packet packet;

        std::cout << "words[0] = " << packet.words[0] << "\n";

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

        std::cout << "\n\nwords[1] = " << packet.words[1] << "\n";

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

        std::cout << "\n\nwords[2] = " << packet.words[2] << "\n";

        try {
                userInputToThirdWord(1, 1, packet.words[2]);
                std::cout << "1 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(-12.7, 0, packet.words[2]);
                std::cout << "2 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(12.8, 0, packet.words[2]);
                std::cout << "3 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(0, 130, packet.words[2]);
                std::cout << "4 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(0, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";

                userInputToThirdWord(-12.8, 0, packet.words[2]);
                std::cout << "5 words[2] = " << packet.words[2] << "\n";
        }
        catch(std::runtime_error& error) {
                std::cout << "catch " << error.what() << "\n";
        }

        std::cout << "\n\nwords[3] = " << packet.words[3] << "\n";

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

        std::cout << "sizeof = " << sizeof(int8_t) << "\n";
}