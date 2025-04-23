#include "coder.h"

namespace coder {

    void Coder::setCoderData(const std::vector<byte> &poly, const byte &n, const std::vector<byte> &msgVec)
    {
        m_polynom = poly;
        m_msgSize = msgVec.size();
        m_msgVector = msgVec;
        m_degree = m_polynom.size() - 1;
    }

    const std::vector<byte> &Coder::codeWord()
    {
        std::vector<byte> a = m_msgVector;
        a.resize(m_msgVector.size() + m_degree, 0);

        std::vector<byte> remainder = a;

        for (auto i = 0; i <= a.size() - m_polynom.size(); ++i) {
            if (remainder[i] == 1) {
                for (auto j = 0; j < m_polynom.size(); ++j) {
                    remainder[i + j] ^= m_polynom[j];
                }
            }
        }

        std::vector<byte> checkSum(remainder.end() - m_degree, remainder.end());

        std::vector<byte> codeWord = m_msgVector;
        codeWord.insert(codeWord.end(), checkSum.begin(), checkSum.end());

        std::vector<byte> checkCodeWord = codeWord;
        for (auto i = 0; i <= codeWord.size() - m_polynom.size(); ++i) {
            if (checkCodeWord[i] == 1) {
                for (auto j = 0; j < m_polynom.size(); ++j) {
                    checkCodeWord[i + j] ^= m_polynom[j];
                }
            }
        }

        for (const auto pow : checkCodeWord)
            if (pow != 0)
                return {};

        return codeWord;
    }
}
