#pragma once
#include "coder.h"

namespace coder {
    class Decoder {

    public:
        explicit Decoder() = default;
        ~Decoder() = default;
        void setDecoderData(std::vector<byte> &poly, const byte &k, std::vector<byte> &codeWord, std::vector<byte> &errorVector);
        bool makeDecision();
        std::vector<byte> getSyndrome() { return m_syndrome; }
        std::vector<byte> getB() { return m_b; }
    private:
        std::vector<byte> m_polynom;
        std::vector<byte> m_codeWord;
        std::vector<byte> m_errorVector;
        byte m_degree;
        byte m_len;
        std::vector<byte> m_b;
        std::vector<byte> m_syndrome;
    };
}