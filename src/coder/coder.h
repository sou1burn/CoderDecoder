#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>

namespace coder {
    using byte = uint8_t;
    class Coder {

    public:
        explicit Coder() = default;
        ~Coder() = default;
        void setCoderData(const std::vector<byte> &poly, const byte &n, const std::vector<byte> &msgVec);
        const std::vector<byte> &codeWord();
    private:
        std::vector<byte> m_polynom;
        std::vector<byte> m_msgVector;
        byte m_msgSize;
        byte m_degree;
    };
}