#pragma once
#include "coder.h"

namespace coder {
    class Decoder {

    public:
        explicit Decoder() = default;
        ~Decoder() = default;
        void setDecoderData(std::vector<byte> &poly, const byte &k, std::vector<byte> &codeWord);
        bool makeDecision();
    private:
        std::vector<byte> m_polynom;
        byte m_msgSize;
        std::vector<byte> m_codeWord;
        byte m_degree;
        std::vector<byte> m_codeWord;
    };
}