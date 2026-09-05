#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

const int MIN_CODE_RUSSIAN_LETTERS = 0xC0;
const int MAX_CODE_RUSSIAN_LETTERS = 0xFF;
const int CODE_UPPERCASE_E = 0xA8;
const int CODE_LOWERCASE_E = 0xB8;

const std::string STANDART_RESPONSE = "\xD1\xEF\xE0\xF1\xE8\xE1\xEE \xE7\xE0 \xE8\xED\xF4\xEE\xF0\xEC\xE0\xF6\xE8\xFE";
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

bool IsPunctuation(std::string itemSentence) {
    return itemSentence == "." || itemSentence == "!" || itemSentence == "?";
}

void UpdateLastSentence(std::vector<std::string> &lastSentence, const std::vector<std::string> &subwords) {
    for (int i = 0; i < subwords.size(); i++) {
        if (lastSentence.size() > 1 && IsPunctuation(lastSentence[lastSentence.size() - 1]) && !IsPunctuation(subwords[i])) {
            lastSentence.clear();
            lastSentence.push_back(subwords[i]);
        } else {
            lastSentence.push_back(subwords[i]);
        }
    }
}

std::vector<std::string> GetLastSentenceFromFile(std::ifstream &infile) {
    std::string word;
    std::vector<std::string> lastSentence;
    while (infile >> word) {
        std::vector<std::string> subwords = SplitWord(word);
        UpdateLastSentence(lastSentence, subwords);
    }
    return lastSentence;
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

bool CheckTargetWord(const std::string word) {
    if (word.length() < 3 || word.length() > 3) {
        return false;
    }

    unsigned char firstLetter = static_cast<unsigned char>(word[0]);
    if (firstLetter == 0xC3 || firstLetter == 0xE3) {
        return true;
    }

    unsigned char secondLetter = static_cast<unsigned char>(word[1]);
    if (secondLetter == 0xC4 || secondLetter == 0xE4) {
        return true;
    }

    unsigned char thirdLetter = static_cast<unsigned char>(word[2]);
    if (thirdLetter == 0xC5 || thirdLetter == 0xE5) {
        return true;
    }

    return false;
}

void ReplaceTargetSymbols(std::vector<std::string> &lastSentence) {
    for (int i = 0; i < lastSentence.size(); i++) {
        if (IsPunctuation(lastSentence[i])) {
            lastSentence[i] = REPLACEMENT_SYMBOL;
        }
    }
}

std::string GenereteResponseFromFile(std::ifstream &infile) {
    std::string response;
    std::vector<std::string> lastSentence = GetLastSentenceFromFile(infile);
    if (!lastSentence.empty()) {
        if (CheckTargetWord(lastSentence.front()) && lastSentence.back() == TARGET_SYMBOL) {
            lastSentence.front() = REPLACEMENT_WORD;
            ReplaceTargetSymbols(lastSentence);
            response = WriteArrToStr(lastSentence);
        } else {
            response = STANDART_RESPONSE;
        }
    }
    return response;
}

int main() {
    std::string nameFileInput;
    std::cout << "Введите названия INPUT файла: ";
    std::cin >> nameFileInput;
    std::string nameFileOutput;
    std::cout << "Введите название OUTPUT файла: ";
    std::cin >> nameFileOutput;

    std::string response;
    std::ifstream infile;
    infile.open(nameFileInput);
    if (infile.is_open()) {
        response = GenereteResponseFromFile(infile);
        infile.close();
    } else {
        std::cerr << "Ошибка чтения INPUT файла!!!" << std::endl;
        return 1;
    }

    std::ofstream outfile;
    outfile.open(nameFileOutput);
    if (outfile.is_open()) {
        outfile << response << std::endl;
        outfile.close();
    } else {
        std::cerr << "Ошибка записи OUTPUT файла!!!" << std::endl;
        return 1;
    }
}