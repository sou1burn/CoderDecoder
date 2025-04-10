#include <chrono>
#include "decoder.h"

namespace helpers {
    std::vector<coder::byte> parseBinaryString(const std::string& binaryStr)
    {
        std::vector<coder::byte> result;
        for (const char bit : binaryStr) {
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
            auto count = 0;
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
        const size_t numMessages = 1 << len;
        for (auto i = 0; i < numMessages; ++i) {
            std::vector<coder::byte> msg(len, 0);
            for (auto j = 0; j < len; ++j) {
                if (i & (1 << j))
                    msg[j] = 1;
            }
            messages.push_back(msg);
        }
        return messages;
    }

    std::string printByteVector(const std::vector<coder::byte>& vec) {
        std::string result;
        for (const auto &byte : vec)
            result += std::to_string(static_cast<int>(byte));
        return result;
    }
} // namespace helpers

int main() {
    std::string polynom, n, errVec, sequence;

    std::cout << "Enter g(x): ";
    std::cin >> polynom;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter error vector: ";
    std::cin >> errVec;
    std::cout << "Enter m: ";
    std::cin >> sequence;

    auto polynomVec = helpers::parseBinaryString(polynom);
    auto errorVector = helpers::parseBinaryString(errVec);
    auto msgVector = helpers::parseBinaryString(sequence);
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
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::cout << "----------/ ТУТ НАЧИНАЕТСЯ ДОП /-------------" << std::endl;
    std::cout << "Пусть порождающий многочлен равен fi(x) * (x+1) = (x^3 + x + 1) * (x + 1) = x^4 + x^3 + x^2 + 1, а n было введено ранее" << std::endl;
    std::cout << "Генерируем 2^(n - r) сообщений и 2^n / 2 векторов ошибок" << std::endl;
    start = std::chrono::system_clock::now();

    gx = {1, 1, 1, 0, 1}; // g(x) = x^4 + x^3 + x^2 + 1
    auto count = 0;
    coder::Coder dopCoder;
    coder::Decoder dopDecoder;
    auto oddErrorVectors = helpers::generateOddErrorVector(nValue);
    std::vector<std::pair<std::vector<coder::byte>, std::vector<coder::byte>>> messagesAndErrorVectors;
    std::vector<std::pair<std::vector<coder::byte>, std::vector<coder::byte>>> bsAndSyndroms;
    std::vector<std::vector<coder::byte>> codeWords;
    auto messages = helpers::generateMessages(nValue - (gx.size() - 1));

    for (auto msg : messages) {
        // std::cout << "Сообщение сейчас:" << std::endl;
        // for (auto byte : msg)
        //     std::cout << static_cast<int>(byte);
        // std::cout << std::endl;

        dopCoder.setCoderData(gx, nValue, msg);
        auto codeWordDop = dopCoder.codeWord();
        codeWords.push_back(codeWordDop);
        for (auto &errorVec : oddErrorVectors) {
            // std::cout << "Текущий вектор ошибок:" << std::endl;
            // for (auto byte : errorVec)
            //     std::cout << static_cast<int>(byte);
            // std::cout << std::endl;

            dopDecoder.setDecoderData(gx, codeWordDop.size(), codeWordDop, errorVec);
            bool dopResult = dopDecoder.makeDecision();

            if (dopResult) {
                // std::cout << "Ошибка не обнаружена для вектора: ";
                // for (auto byte : errorVec)
                //     std::cout << static_cast<int>(byte);
                // std::cout << std::endl;
                messagesAndErrorVectors.emplace_back(msg, errorVec);
            } else
                count++;
        }
    }
    // for (auto w : codeWords)
    //     std::cout << "Кодовое слово: " << helpers::printByteVector(w) << std::endl;

    std::cout << "Статистика для g(x) = x^4 + x^3 + x^2 + 1"<< std::endl;
    std::cout << "Всего не найдено ошибок:" << messagesAndErrorVectors.size() << std::endl;
    std::cout << "Всего найдено ошибок:" << count << std::endl;
    std::cout << "Пары сообщение - вектор ошибок, для которых не были обнаружены ошибки: " << std::endl;

    for (const auto &pair : messagesAndErrorVectors)
        std::cout << "Сообщение m: " << helpers::printByteVector(pair.first) << " Вектор ошибок e: " << helpers::printByteVector(pair.second) << std::endl;

    for (auto pair : messagesAndErrorVectors) {
        dopCoder.setCoderData(gx, nValue, pair.first);
        auto codeWordDop = dopCoder.codeWord();
        dopDecoder.setDecoderData(gx, codeWordDop.size(), codeWordDop, pair.second);
        bsAndSyndroms.push_back({dopDecoder.getB(), dopDecoder.getSyndrome()});
    }

    // std::cout << "Пары b(x) и s(x): " << std::endl;
    // for (const auto &pair : bsAndSyndroms)
    //     std::cout << "b(x): " << helpers::printByteVector(pair.first) << " s(x): " << helpers::printByteVector(pair.second) << std::endl;
    auto count1 = 0;
    for (const auto &pair : messagesAndErrorVectors) {
        auto rem1 = pair.second;
        for (auto bit = 0; bit < rem1.size() - (gx.size() - 1); ++bit) {
            if (rem1[bit] == 1) {
                for (auto j = 0; j < gx.size(); ++j) {
                    rem1[bit + j] ^= gx[j];
                }
            }
        }
        for (auto j = 0; j < rem1.size(); ++j) {
            if (rem1[j] == 1) {
                count1++;
                break;
            }
        }
    }

    std::cout << "Число неделящихся на g(x) векторов ошибок " << count1 << std::endl;
    std::cout << "\n----------------------------------------------------------------------\n"<< std::endl;
    std::cout << "\n________________________________________________________________________\n"<< std::endl;

    std::cout << "Пусть порождающий многочлен равен fi(x) * (x+1) = (x^3 + x^2 + 1) * (x + 1) = x^4 + x^2 + x + 1, а n было введено ранее"<< std::endl;
    gx = {1, 0, 1, 1, 1}; // g(x) = x^4 + x^2 + x + 1
    messagesAndErrorVectors.clear();
    bsAndSyndroms.clear();
    codeWords.clear();

    count = 0;

    for (auto message : messages) {
        // std::cout << "Сообщение сейчас:" << std::endl;
        // for (auto byte : message)
        //     std::cout << static_cast<int>(byte);
        // std::cout << std::endl;

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
                // std::cout << "Ошибка не обнаружена для вектора: ";
                // for (auto byte : errorVec)
                //     std::cout << static_cast<int>(byte);
                // std::cout << std::endl;
                messagesAndErrorVectors.push_back({message, errorVec});
            } else
                count++;
        }
    }

    // for (auto w : codeWords)
    //     std::cout << "Кодовое слово: " << helpers::printByteVector(w) << std::endl;

    std::cout << "Статистика для g(x) = x^4 + x^2 + x + 1"<< std::endl;
    std::cout << "Всего не найдено ошибок:" << messagesAndErrorVectors.size() << std::endl;
    std::cout << "Всего найдено ошибок:" << count << std::endl;
    std::cout << "Пары сообщение - вектор ошибок, для которых не были обнаружены ошибки: " << std::endl;

    for (const auto &pair : messagesAndErrorVectors)
        std::cout << "Сообщение m: " << helpers::printByteVector(pair.first) << " Вектор ошибок e: " << helpers::printByteVector(pair.second) << std::endl;
    end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Время работы: " << duration.count() << " мс" << std::endl;

    for (auto pair : messagesAndErrorVectors) {
        dopCoder.setCoderData(gx, nValue, pair.first);
        auto codeWordDop = dopCoder.codeWord();
        dopDecoder.setDecoderData(gx, codeWordDop.size(), codeWordDop, pair.second);
        bsAndSyndroms.push_back({dopDecoder.getB(), dopDecoder.getSyndrome()});
    }

    // std::cout << "Пары b(x) и s(x): " << std::endl;
    // for (const auto &pair : bsAndSyndroms)
    //     std::cout << "b(x): " << helpers::printByteVector(pair.first) << " s(x): " << helpers::printByteVector(pair.second) << std::endl;
    // std::cout << "Все сообщения: " << std::endl;
    // for (auto msg :messages)
    //     std::cout << helpers::printByteVector(msg) << std::endl;
    // std::cout << "Количество сообщений: " << messages.size() << std::endl;
    std::vector<coder::byte> needed = {0, 0, 0, 1, 0, 1, 1, 0};// 00010110
    std::cout << "error vector: " << helpers::printByteVector(needed) << std::endl;
    auto rem = needed;
    for (auto bit = 0; bit < rem.size() - (gx.size() - 1); ++bit) {
        if (rem[bit] == 1) {
            for (auto j = 0; j < gx.size(); ++j) {
                rem[bit + j] ^= gx[j];
            }
        }
    }
    std::cout << "res = " << helpers::printByteVector(rem) << std::endl;

    count1 = 0;

    for (const auto &pair : messagesAndErrorVectors) {
        auto rem1 = pair.second;
        for (auto bit = 0; bit < rem1.size() - (gx.size() - 1); ++bit) {
            if (rem1[bit] == 1) {
                for (auto j = 0; j < gx.size(); ++j) {
                    rem1[bit + j] ^= gx[j];
                }
            }
        }

        for (auto j = 0; j < rem1.size(); ++j) {
            if (rem1[j] == 1) {
                count1++;
                break;
            }
        }
    }

    std::cout << "Число неделящихся на g(x) векторов ошибок " << count1 << std::endl;
    return 0;

}
