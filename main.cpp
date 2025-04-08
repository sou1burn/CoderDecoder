#include "decoder.h"

std::vector<coder::byte> parseBinaryString(const std::string& binaryStr)
{
    std::vector<coder::byte> result;
    for (char bit : binaryStr) {
        if (bit == '0' || bit == '1')
            result.push_back(bit - '0');
        else
           throw std::runtime_error("Error: Only binary input allowed");

    }

    return result;
}

std::vector<std::vector<coder::byte>> generateOddErrorVector(const int &len)
{
    std::vector<std::vector<coder::byte>> oddErrorVectors;
    const size_t numVectors = 1 << len;
    for (size_t i = 1; i < numVectors; ++i) {
        std::vector<coder::byte> errorVector(len, 0);
        int count = 0;
        for (size_t j = 0; j < len; ++j) {
            if (i & (1 << j)) {
                errorVector[j] = 1;
                ++count;
            }
        }
        if (count % 2 == 1)
            oddErrorVectors.push_back(errorVector);
    }
    return oddErrorVectors;
}

std::vector<std::vector<coder::byte>> generateMessages(const int &len)
{
    std::vector<std::vector<coder::byte>> messages;
    const size_t numVectors = 1 << len;
    for (auto i = 0; i < numVectors; ++i) {
        std::vector<coder::byte> msg(len, 0);
        for (auto j = 0; j < len; ++j) {
            if (i & (1 << j))
                msg[j] = 1;
        }
        messages.push_back(msg);
    }
    return messages;
}

int main() {
    std::string polynom, n, errVec, sequence;
    //, option;

    std::cout << "Enter g(x): ";
    std::cin >> polynom;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter error vector: ";
    std::cin >> errVec;
    std::cout << "Enter m: ";
    std::cin >> sequence;

    auto polynomVec = parseBinaryString(polynom);
    auto errorVector = parseBinaryString(errVec);
    auto msgVector = parseBinaryString(sequence);
    const coder::byte nValue = std::stoi(n);

    coder::Coder coder;
    coder.setCoderData(polynomVec , nValue, msgVector);
    auto codeWord = coder.codeWord();
    std::cout << "a (code word): ";
    for (auto byte : codeWord)
        std::cout << static_cast<int>(byte);

    std::cout << std::endl;
    coder::Decoder decoder;
    decoder.setDecoderData(polynomVec, nValue, codeWord, errorVector);

    bool result = decoder.makeDecision();
    auto syndrome = decoder.getSyndrome();
    auto b = decoder.getB();

    std::cout << "received message b (a ^ e): ";
    for (auto byte : b)
        std::cout << static_cast<int>(byte);
    std::cout << std::endl;

    std::cout << "syndrome : ";
    for (auto byte : syndrome)
        std::cout << static_cast<int>(byte);
    std::cout << std::endl;

    std::cout << "Decision: " << (result ? "Error not detected" : "Error detected") << std::endl;

    // std::cout << "Enter option (1: fi(x) = x^3 + x + 1; 2: fi(x) = x^3 + x^2 + 1): ";
    // std::cin >> option;

    std::vector<coder::byte> gx(5);
    // if (option == "1") {
    //     // fix = {1, 0, 1, 1} * {1, 1};
    //     gx = {1, 1, 1, 0, 1}; // g(x) = x^4 + x^3 + x^2 + 1
    // } else if (option == "2") {
    //     // fix = {1, 1, 0, 1} * {1, 1};
    //     gx = {1, 0, 1, 1, 1}; // g(x) = x^4 + x^2 + x + 1
    // } else {
    //     std::cerr << "Invalid option" << std::endl;
    //     return 1;
    // }
    std::cout << "----------/ ТУТ НАЧИНАЕТСЯ ДОП /-------------" << std::endl;
    std::cout << "Пусть порождающий многочлен равен fi(x) * (x+1) = (x^3 + x + 1) * (x + 1) = x^4 + x^3 + x^2 + 1, а n было введено ранее" << std::endl;
    std::cout << "Генерируем 2^(n -r) сообщений и 2^n / 2 векторов ошибок" << std::endl;
    gx = {1, 1, 1, 0, 1}; // g(x) = x^4 + x^3 + x^2 + 1
    coder::Coder dopCoder;
    coder::Decoder dopDecoder;
    auto oddErrorVectors = generateOddErrorVector(nValue);
    std::vector<std::pair<std::vector<coder::byte>, std::vector<coder::byte>>> messagesAndErrorVectors;

    auto messages = generateMessages(nValue - (gx.size() - 1));

    for (auto msg : messages) {
        std::cout << "Сообщение сейчас:" << std::endl;
        for (auto byte : msg)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;

        dopCoder.setCoderData(gx, nValue, msg);
        auto codeWordDop = dopCoder.codeWord();

        for (auto &errorVec : oddErrorVectors) {
            // std::cout << "Текущий вектор ошибок:" << std::endl;
            // for (auto byte : errorVec)
            //     std::cout << static_cast<int>(byte);
            // std::cout << std::endl;

            dopDecoder.setDecoderData(gx, codeWordDop.size(), codeWord, errorVec);
            bool dopResult = dopDecoder.makeDecision();

            if (dopResult) {
                std::cout << "Ошибка не обнаружена для вектора: ";
                for (auto byte : errorVec)
                    std::cout << static_cast<int>(byte);
                std::cout << std::endl;
                messagesAndErrorVectors.push_back({msg, errorVec});
            }
        }
    }

    std::cout << "Статистика для g(x) = x^4 + x^3 + x^2 + 1"<< std::endl;
    std::cout << "Всего найдено ошибок:" << messagesAndErrorVectors.size() << std::endl;
    std::cout << "Пары сообщение - вектор ошибок, для которых не были обнаружены ошибки: " << std::endl;
    std::cout << "Сообщение m: ";

    for (const auto &pair : messagesAndErrorVectors) {
        for (auto byte : pair.first)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;
    }
    std::cout << "Вектор ошибок e: ";
    for (const auto &pair : messagesAndErrorVectors) {
        for (auto byte : pair.second)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;
    }

    std::cout << "Пусть порождающий многочлен равен fi(x) * (x+1) = (x^3 + x^2 + 1) * (x + 1) = x^4 + x^2 + x + 1, а n было введено ранее"<< std::endl;
    gx = {1, 0, 1, 1, 1}; // g(x) = x^4 + x^2 + x + 1
    messagesAndErrorVectors.clear();

    for (auto message : messages) {
        std::cout << "Сообщение сейчас:" << std::endl;
        for (auto byte : message)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;

        dopCoder.setCoderData(gx, nValue, message);
        auto codeWordDop1 = dopCoder.codeWord();

        for (auto &errorVec : oddErrorVectors) {
            // std::cout << "Текущий вектор ошибок:" << std::endl;
            // for (auto byte : errorVec)
            //     std::cout << static_cast<int>(byte);
            // std::cout << std::endl;

            dopDecoder.setDecoderData(gx, codeWordDop1.size(), codeWordDop1, errorVec);
            bool dopResult = dopDecoder.makeDecision();

            if (dopResult) {
                std::cout << "Ошибка не обнаружена для вектора: ";
                for (auto byte : errorVec)
                    std::cout << static_cast<int>(byte);
                std::cout << std::endl;
                messagesAndErrorVectors.push_back({message, errorVec});
            }
        }
    }

    std::cout << "Статистика для g(x) = x^4 + x^2 + x + 1"<< std::endl;
    std::cout << "Всего найдено ошибок:" << messagesAndErrorVectors.size() << std::endl;
    std::cout << "Пары сообщение - вектор ошибок, для которых не были обнаружены ошибки: " << std::endl;
    std::cout << "Сообщение m: ";

    for (const auto &pair : messagesAndErrorVectors) {
        for (auto byte : pair.first)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;
    }
    std::cout << "Вектор ошибок e: ";
    for (const auto &pair : messagesAndErrorVectors) {
        for (auto byte : pair.second)
            std::cout << static_cast<int>(byte);
        std::cout << std::endl;
    }
    return 0;

}
