#include <iostream>
#include "decoder.h"

std::vector<byte> parseBinaryString(const std::string& binaryStr) {
    std::vector<byte> result;
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
    coder::byte k = __argv[2];
    std::string errVec = __argv[3];
    std::string sequence = __argv[4];
    auto polyVec = parseBinaryString(polynom);
    auto errVec = parseBinaryString(errVec);
    auto msgVec = parseBinaryString(sequence);

    coder::Coder coder;
    coder.setCoderData(&polyVec, &k, &errVec, &msgVec);

    return 0;
}
