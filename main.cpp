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

    std::string polynom = __argv[1];
    std::string k = __argv[2];
    std::string errVec = __argv[3];
    std::string sequence = __argv[4];
    auto polynomVec = parseBinaryString(polynom);
    auto errorVector = parseBinaryString(errVec);
    auto msgVector = parseBinaryString(sequence);
    coder::byte kValue = std::stoi(k);

    coder::Coder coder;
    coder.setCoderData(polynomVec, kValue, errorVector, msgVector);
    coder::Decoder decoder;
    decoder.setDecoderData(polynomVec, kValue, coder.codeWord());

    return 0;
}
