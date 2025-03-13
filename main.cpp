#include "decoder.h"

std::vector<coder::byte> parseBinaryString(const std::string& binaryStr) {
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

int main() {
    std::string polynom, k, errVec, sequence;

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
    coder.setCoderData(polynomVec, kValue, msgVector);
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


    return 0;
}
