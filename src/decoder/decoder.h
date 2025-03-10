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
        std::vector<byte> m_codeWord;
        byte m_degree;
        byte m_len;
        std::vector<byte> m_errorVector;
    };
}