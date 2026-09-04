#include <iostream>
#include <fstream>
#include <vector>
#include <locale>

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

std::vector<std::string> SplitWord(std::string word) {
    std::vector<std::string> subwords;
    std::string tempSubword;

    for (int i = 0; i < word.length(); i++) {
        if (std::isalpha(word[i])) {
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
            if (lastOffer.back() == "?" && lastOffer.front() == "Where") {
                lastOffer.front() = "В Караганде";
                lastOffer.back() = "!";
                responce = WriteArrToStr(lastOffer);
            } else {
                responce = "Спасибо за информацию";
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