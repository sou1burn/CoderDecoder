#include "decoder.h"

std::vector<coder::byte> parseBinaryString(const std::string& binaryStr)
{
    std::vector<coder::byte> result;
    for (char bit : binaryStr) {
        if (bit == '0' || bit == '1') {
            result.push_back(bit - '0');
        } else {
           throw std::runtime_error("Error: Only binary input allowed");
        }
    }

    return result;
}

std::vector<std::vector<coder::byte>> generateOddErrorVector(int len)
{
    std::vector<std::vector<coder::byte>> oddErrorVectors;
    size_t numVectors = 1 << len;
    for (auto i = 0; i < numVectors; i += 2) {
        std::vector<coder::byte> errorVector(len, 0);
        for (auto j = 0; j < len; ++j) {
            if (i & (1 << j)) {
                errorVector[j] = 1;
            }
        }
        oddErrorVectors.push_back(errorVector);
    }
    return oddErrorVectors;
}

int main() {
    std::string polynom, k, errVec, sequence, option;

    std::cout << "Enter g(x): ";
    std::cin >> polynom;
    std::cout << "Enter n: ";
    std::cin >> k;
    std::cout << "Enter error vector: ";
    std::cin >> errVec;
    std::cout << "Enter m: ";
    std::cin >> sequence;

    auto polynomVec = parseBinaryString(polynom);
    auto errorVector = parseBinaryString(errVec);
    auto msgVector = parseBinaryString(sequence);
    const coder::byte kValue = std::stoi(k);

    coder::Coder coder;
    coder.setCoderData(polynomVec , kValue, msgVector);
    auto codeWord = coder.codeWord();
    std::cout << "a (code word): ";
    for (auto byte : codeWord)
        std::cout << static_cast<int>(byte);

    std::cout << std::endl;
    coder::Decoder decoder;
    decoder.setDecoderData(polynomVec, kValue, codeWord, errorVector);

    bool result = decoder.makeDecision();
    auto syndrome = decoder.getSyndrome();
    auto b = decoder.getB();

    std::cout << "received message b (a ^ e): ";
    for (auto byte : b)
        std::cout << static_cast<int>(byte);
    std::cout << std::endl;

    std::cout << "syndrome : ";
    for (auto byte : syndrome)
        std::cout << static_cast<int>(byte);
    std::cout << std::endl;

    std::cout << "Decision: " << (result ? "Error not detected" : "Error detected") << std::endl;

    std::cout << "Enter option (1: fi(x) = x^3 + x + 1; 2: fi(x) = x^3 + x^2 + 1): ";
    std::cin >> option;

    std::vector<coder::byte> gx(5);
    if (option == "1") {
        // fix = {1, 0, 1, 1};
        gx = {1, 1, 1, 0, 1};
    } else if (option == "2") {
        // fix = {1, 1, 0, 1};
        gx = {1, 0, 1, 1, 1};
    } else {
        std::cerr << "Invalid option" << std::endl;
        return 1;
    }

    coder::Coder dopCoder;
    dopCoder.setCoderData(gx, kValue, msgVector);
    auto codeWordDop = dopCoder.codeWord();

    auto oddErrorVectors = generateOddErrorVector(codeWordDop.size());
    coder::Decoder dopDecoder;

    for (auto &errorVec : oddErrorVectors) {
        dopDecoder.setDecoderData(gx, codeWordDop.size(), codeWord, errorVec);
        bool dopResult = dopDecoder.makeDecision();

        if (dopResult) {
            std::cout << "Error not detected for error vector: ";
            for (auto byte : errorVector)
                std::cout << static_cast<int>(byte);
            std::cout << std::endl;
        }
    }

    return 0;
}
