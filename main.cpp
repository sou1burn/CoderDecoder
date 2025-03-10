#include <iostream>
#include "decoder.h"

std::vector<coder::byte> parseBinaryString(const std::string& binaryStr) {
    std::vector<coder::byte> result;
    for (char bit : binaryStr) {
        if (bit == '0' || bit == '1') {
            result.push_back(bit - '0');
        } else {
            continue;
        }
    }

    return result;
}

int main(int __argc, char** __argv) {
    if (__argc != 5)
        std::cerr << "Запуск программы: " << __argv[0] << " <полином> <k> <вектор ошибок> <последовательность>" << std::endl;

    std::string polynom = __argv[1];
    std::string k = __argv[2];
    std::string errVec = __argv[3];
    std::string sequence = __argv[4];

    auto polynomVec = parseBinaryString(polynom);
    auto errorVector = parseBinaryString(errVec);
    auto msgVector = parseBinaryString(sequence);
    const coder::byte kValue = std::stoi(k);

    std::cout << "Polynomial: " << polynom << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "Error Vector: " << errVec << std::endl;
    std::cout << "Message Sequence: " << sequence << std::endl;

    coder::Coder coder;
    coder.setCoderData(polynomVec, kValue, msgVector);
    auto codeWord = coder.codeWord();
    std::cout << "Code Word: ";
    for (auto byte : codeWord)
        std::cout << static_cast<int>(byte);

    std::cout << std::endl;
    coder::Decoder decoder;
    decoder.setDecoderData(polynomVec, kValue, codeWord, errorVector);

    bool result = decoder.makeDecision();
    std::cout << "Decision: " << (result ? "Error detected" : "Error not detected") << std::endl;

    return 0;
}
