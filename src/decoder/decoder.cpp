#include "decoder.h"
//dop C
namespace coder {
    void Decoder::setDecoderData(const std::vector<byte> &poly, const byte &n, const std::vector<byte> &codeWord, const std::vector<byte> &errorVector)
    {
        m_polynom = poly;
        m_codeWord = codeWord;
        m_degree = m_polynom.size() - 1;
        m_len = n;
        m_errorVector = errorVector;
    }

       bool Decoder::makeDecision() {
        if (m_codeWord.size() != m_len)
            return false;

        m_b.resize(m_codeWord.size());
        for (auto i = 0; i < m_codeWord.size(); ++i)
            m_b[i] = m_codeWord[i] ^ m_errorVector[i];

        m_syndrome.resize(m_b.size(), 0);
        std::vector<coder::byte> remainder = m_b;
        // std::cout << "\nb(x): ";
        // for (auto byte : m_b) std::cout << static_cast<int>(byte);
        // std::cout << "\n";
        //
        // std::cout << "g(x): ";
        // for (auto byte : m_polynom) std::cout << static_cast<int>(byte);
        // std::cout << "\n\n";

        for (size_t i = 0; i <= remainder.size() - m_polynom.size(); ++i) {
            if (remainder[i] == 1) {
                // std::cout << "Step " << i << ":\n";
                //
                // std::cout << "m_b before step: ";
                // for (auto byte : m_b) std::cout << static_cast<int>(byte);
                // std::cout << "\n";
                // std::cout << "g(x): ";
                // for (auto byte : m_polynom) std::cout << static_cast<int>(byte);
                // std::cout << "\n";

                for (size_t j = 0; j < m_polynom.size(); ++j) {
                    remainder[i + j] ^= m_polynom[j];
                }

                // std::cout << "m_b after step:  ";
                // for (auto byte : m_b) std::cout << static_cast<int>(byte);
                // std::cout << "\n";
            }
        }
        m_syndrome.resize(m_polynom.size() - 1, 0);
        std::copy(remainder.end() - m_syndrome.size(), remainder.end(), m_syndrome.begin());
        // g(x):1110
        // n:8
        // error vector:10110111
        // m:11101
        for (const auto pow : m_syndrome)
            if (pow != 0)
                return false;

        return true;
    }
}
