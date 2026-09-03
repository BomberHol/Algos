#include <iostream>
#include <fstream>
#include <vector>

void WriteArr(std::vector<std::string> arr) {
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << std::endl;
    }
}

std::vector<std::string> SplitWord(std::string word) {
    std::vector<std::string> subwords;
    std::string tempSubword;
    for (int i = 0; i < word.length(); i++) {
        if ((word[i] >= 'A' && word[i] <= 'Z') || (word[i] >= 'A' && word[i] <= 'Z')) {
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



int main() {
    std::ifstream infile;
    infile.open("text.txt");

    if (infile.is_open()) {
        std::string word;
        std::vector<std::string> words(0);

        while (infile >> word) {
            std::vector<std::string> subwords = SplitWord(word);
            for (int i = 0; i < subwords.size(); i++) {
                words.push_back(subwords[i]);
            }
        }
        WriteArr(words);
    }
    infile.close();
}

