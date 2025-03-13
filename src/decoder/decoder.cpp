#include "decoder.h"

namespace coder {
    void Decoder::setDecoderData(std::vector<byte> &poly, const byte &k, std::vector<byte> &codeWord, std::vector<byte> &errorVector)
    {
        m_polynom = poly;
        m_codeWord = codeWord;
        m_degree = m_polynom.size() - 1;
        m_len = k;
        m_errorVector = errorVector;
    }

    bool Decoder::makeDecision() {
        if (m_codeWord.size() != m_len)
            return false;

        m_b.resize(m_codeWord.size());
        for (auto i = 0; i < m_codeWord.size(); ++i)
            m_b[i] = m_codeWord[i] ^ m_errorVector[i];

        m_syndrome.resize(m_b.size(), 0);

        for (auto i = 0; i <= m_b.size() - m_polynom.size(); ++i) {
            if (m_b[i] == 1) {
                for (auto j = 0; j < m_polynom.size(); ++j) {
                    m_syndrome[i + j] ^= m_polynom[j];
                }
            }
        }

        for (const auto pow : m_syndrome)
            if (pow != 0)
                return false;

        return true;
    }
}
