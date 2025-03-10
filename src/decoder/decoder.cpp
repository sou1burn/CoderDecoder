#include "decoder.h"

namespace coder {
    void Decoder::setDecoderData(std::vector<byte> &poly, const byte &k, std::vector<byte> &codeWord)
    {
        m_polynom = poly;
        m_codeWord = codeWord;
        m_degree = m_polynom.size() - 1;
        m_len = k;
    }


    bool Decoder::makeDecision() {
        if (m_codeWord.size() != m_len)
            return false;

        std::vector<byte> b(m_codeWord.size());
        for (auto i = 0; i < m_codeWord.size(); ++i)
            b[i] = m_codeWord[i] ^ m_errorVector[i];

        std::vector<byte> syndrome(b.size(), 0);

        for (auto i = 0; i <= b.size() - m_polynom.size(); ++i) {
            if (b[i] == 1) {
                for (auto j = 0; j < m_polynom.size(); ++j) {
                    syndrome[i + j] ^= m_polynom[j];
                }
            }
        }

        for (const auto pow : syndrome)
            if (pow != 0)
                return false;

        return true;
    }
}
