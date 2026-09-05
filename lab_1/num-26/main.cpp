#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

const int MIN_CODE_RUSSIAN_LETTERS = 0xC0;
const int MAX_CODE_RUSSIAN_LETTERS = 0xFF;
const int CODE_UPPERCASE_E = 0xA8;
const int CODE_LOWERCASE_E = 0xB8;

const std::string STANDART_RESPONCE = "\xD1\xEF\xE0\xF1\xE8\xE1\xEE \xE7\xE0 \xE8\xED\xF4\xEE\xF0\xEC\xE0\xF6\xE8\xFE";
const std::string TARGET_WORD = "\xC3\xE4\xE5";
const std::string REPLACEMENT_WORD = "\xC2 \xCA\xE0\xF0\xE0\xE3\xE0\xED\xE4\xE5";
const std::string TARGET_SYMBOL = "?";
const std::string REPLACEMENT_SYMBOL = "!";

void WriteArrToCOUT(std::vector<std::string> arr) {
    for (int i = 0; i < arr.size(); i++) {
        if (i == 0 || arr[i] == "!" || arr[i] == "?" || arr[i] == ".") {
            std::cout << arr[i];
        } else {
            std::cout << " " << arr[i];
        }
    }
    std::cout << std::endl;
}

bool CheckEnglishLetter(const unsigned char ch) {
    return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
}

bool CheckRussianLetter(const unsigned char ch) {
    return (ch >= MIN_CODE_RUSSIAN_LETTERS && ch <= MAX_CODE_RUSSIAN_LETTERS) || ch == CODE_UPPERCASE_E || ch == CODE_LOWERCASE_E;
}

bool CheckLetter(const char ch) {
    const unsigned char convertCh = static_cast<unsigned char>(ch);
    return CheckEnglishLetter(convertCh)|| CheckRussianLetter(convertCh);
}

std::vector<std::string> SplitWord(std::string word) {
    std::vector<std::string> subwords;
    std::string tempSubword;

    for (int i = 0; i < word.length(); i++) {
        if (CheckLetter(word[i])) {
            tempSubword += word[i];
        } else {
            if (!tempSubword.empty()) {
                subwords.push_back(tempSubword);
            }
            tempSubword = word[i];
            subwords.push_back(tempSubword);
            tempSubword = "";
        }
    }

    if (!tempSubword.empty()) {
        subwords.push_back(tempSubword);
    }
    return subwords;
}

void UpdateSentence(std::vector<std::string> &lastOffer, const std::vector<std::string> &subwords) {
    for (int i = 0; i < subwords.size(); i++) {
        if (!lastOffer.empty() && (lastOffer[lastOffer.size() - 1] == "." || lastOffer[lastOffer.size() - 1] == "!" || lastOffer[lastOffer.size() - 1] == "?")) {
            lastOffer.clear();
            lastOffer.push_back(subwords[i]);
        } else {
            lastOffer.push_back(subwords[i]);
        }
    }
}

std::vector<std::string> GetLastOfferFromFile(std::ifstream &infile) {
    std::string word;
    std::vector<std::string> lastOffer;
    while (infile >> word) {
        std::vector<std::string> subwords = SplitWord(word);
        UpdateSentence(lastOffer, subwords);
    }
    return lastOffer;
}

std::string WriteArrToStr(std::vector<std::string> arr) {
    std::string str;
    for (int i = 0; i < arr.size(); i++) {
        if (i == 0 || arr[i] == "!" || arr[i] == "?" || arr[i] == ".") {
            str += arr[i];
        } else {
            str += " " + arr[i];
        }
    }
    return str;
}

int main() {
    std::string nameFileInput;
    std::cout << "Введите названия INPUT файла: ";
    std::cin >> nameFileInput;

    std::string nameFileOutput;
    std::cout << "Введите название OUTPUT файла: ";
    std::cin >> nameFileOutput;

    std::string responce;

    std::ifstream infile;
    infile.open(nameFileInput);
    if (infile.is_open()) {
        std::vector<std::string> lastOffer = GetLastOfferFromFile(infile);
        if (!lastOffer.empty()) {
            if (lastOffer.back() == TARGET_SYMBOL && lastOffer.front() == TARGET_WORD) {
                const std::string firstWord = REPLACEMENT_WORD;
                lastOffer.front() = firstWord;
                lastOffer.back() = REPLACEMENT_SYMBOL;
                responce = WriteArrToStr(lastOffer);
            } else {
                responce = STANDART_RESPONCE;
            }
        }
    } else {
        std::cerr << "Ошибка чтения INPUT файла!!!" << std::endl;
    }
    infile.close();

    std::ofstream outfile;
    outfile.open(nameFileOutput);
    if (outfile.is_open()) {
        outfile << responce << std::endl;
    } else {
        std::cerr << "Ошибка записи OUTPUT файла!!!" << std::endl;
    }
    outfile.close();
}