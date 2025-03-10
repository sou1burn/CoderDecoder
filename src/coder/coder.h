#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace coder {
    using byte = uint8_t;
    class Coder {

    public:
        explicit Coder() = default;
        ~Coder() = default;
        void setCoderData(std::vector<byte> &poly, const byte &k, std::vector<byte> &msgVec);
        std::vector<byte> codeWord();
    private:
        std::vector<byte> m_polynom;
        std::vector<byte> m_msgVector;
        byte m_msgSize;
        byte m_degree;
        byte m_len;
    };
}