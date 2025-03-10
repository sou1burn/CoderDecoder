namespace coder {

    bool Decoder::makeDecision() {
        const auto codeWord = Coder::codeWord();
        if (codeWord.size() != m_len)
            return false;

        if (codeWord.size() != m_errorVector.size())
            m_errorVector.resize(codeWord.size());

        std::vector<byte> b(codeWord.size());
        for (auto i = 0; i < codeWord.size(); ++i)
            b[i] = codeWord[i] ^ m_errorVector[i];

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
