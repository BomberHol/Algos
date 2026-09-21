#include <deque>
#include <fstream>
#include <iostream>

const std::string NULL_STR = "";

class Node {
public:
    std::string word;
    Node* next;
    Node* prev;
    Node(const std::string& inWord) {
        word = inWord;
        prev = nullptr;
        next = nullptr;
    }
};

class Deque {
private:
    Node* beg;
    Node* end;

    void RemoveSingleNode() {
        delete beg;
        beg = nullptr;
        end = nullptr;
    }

    void RemoveTail() {
        Node* prev = end->prev;
        delete end;
        end = prev;
        end->next = nullptr;
    }

    void RemoveHead() {
        Node* next = beg->next;
        delete beg;
        beg = next;
        beg->prev = nullptr;
    }

public:
    Deque() {
        beg = nullptr;
        end = nullptr;
    }

    ~Deque() {
        Node* node = beg;
        while (node != nullptr) {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }

    void push_front(const std::string& inWord) {
        if (beg == nullptr) {
            beg = new Node(inWord);
            end = beg;
        } else {
            Node* newNode = new Node(inWord);
            newNode->next = beg;
            beg->prev = newNode;
            beg = newNode;
        }
    }

    void push_back(const std::string& inWord) {
        if (end == nullptr) {
            end = new Node(inWord);
            beg = end;
        } else {
            Node* newNode = new Node(inWord);
            newNode->prev = end;
            end->next = newNode;
            end = newNode;
        }
    }

    void pop_front() {
        if (beg == nullptr) return;
        if (beg == end) {
            RemoveSingleNode();
            return;
        }
        RemoveHead();
    }

    void pop_back() {
        if (beg == nullptr) return;
        if (beg == end) {
            RemoveSingleNode();
            return;
        }
        RemoveTail();
    }

    std::string front() {
        if (beg != nullptr) {
            return beg->word;
        }
        return NULL_STR;
    }

    std::string back() {
        if (end != nullptr) {
            return end->word;
        }
        return NULL_STR;
    }

    int len() {  // dev
        int count = 0;
        Node* node = beg;
        while (node != nullptr) {
            count++;
            node = node->next;
        }
        return count;
    }

    void write() {
        Node* node = beg;
        while (node != nullptr) {
            std::cout << node->word << std::endl;
            node = node->next;
        }
    }
};

bool IsLetter(const char& ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

char GetUppercase(const char& ch) {
    switch (ch) {
        case 'a':
            return 'A';
        case 'b':
            return 'B';
        case 'c':
            return 'C';
        case 'd':
            return 'D';
        case 'e':
            return 'E';
        case 'f':
            return 'F';
        case 'g':
            return 'G';
        case 'h':
            return 'H';
        case 'i':
            return 'I';
        case 'j':
            return 'J';
        case 'k':
            return 'K';
        case 'l':
            return 'L';
        case 'm':
            return 'M';
        case 'n':
            return 'N';
        case 'o':
            return 'O';
        case 'p':
            return 'P';
        case 'q':
            return 'Q';
        case 'r':
            return 'R';
        case 's':
            return 'S';
        case 't':
            return 'T';
        case 'u':
            return 'U';
        case 'v':
            return 'V';
        case 'w':
            return 'W';
        case 'x':
            return 'X';
        case 'y':
            return 'Y';
        case 'z':
            return 'Z';
    }
    return ch;
}

std::string GetUppercaseStr(const std::string& inWord) {
    std::string word;
    for (int i = 0; i < inWord.length(); i++) {
        word += GetUppercase(inWord[i]);
    }
    return word;
}

void FillDeque(std::ifstream& infile, Deque*& deque) {
    char ch;
    std::string word;
    while (infile.get(ch)) {
        if (ch == 'N') {
            ch = 'N';
        }

        if (ch == '/') {
            infile.get(ch);
            if (ch == '/') {
                infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
        }

        if (ch == '\047') {
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '\047');
            continue;
        }

        if (ch == '{') {
            infile.ignore(std::numeric_limits<std::streamsize>::max(), '}');
            continue;
        }

        if (!IsLetter(ch) && !word.empty()) {
            word = GetUppercaseStr(word);
            if (word == "BEGIN" || word == "END" || word == "RECORD" || word == "CASE" || word == "REPEAT" || word == "UNTIL") {
                deque->push_back(word);
            }
            word = "";
        } else if (IsLetter(ch)) {
            word += ch;
        }
    }

    if (!word.empty()) { // нужно вынести
        word = GetUppercaseStr(word);
        if (word == "BEGIN" || word == "END" || word == "RECORD" || word == "CASE" || word == "REPEAT" || word == "UNTIL") {
            deque->push_back(word);
        }
        word = "";
    }
}

void Flash(Deque*& tempDeque , Deque*& deque) {
    while (tempDeque->len() > 0) {
        std::string tempWord = tempDeque->back();
        tempDeque->pop_back();
        deque->push_front(tempWord);
    }
}

std::string GetOperand(Deque*& deque) {
    std::string oper = deque->front();
    deque->pop_front();
    return oper;
}

std::string CheckRecord(Deque*& deque, std::string oper, const int depth) {
    std::string err;
    if (oper == "RECORD") {
        if (depth == 0) {
            oper = GetOperand(deque);
            if (oper != "END") err = "error ending RECORD";
        } else {
            err = "error nesting RECORD";
        }
    }
    return err;
}

std::string CheckCase(Deque*& deque, std::string oper, const int depth) {
    std::string err;
    if (oper == "CASE") {
        if (depth >= 1) {
            oper = GetOperand(deque);
            if (oper != "END") err = "error ending CASE";
        } else {
            err = "error nesting CASE";
        }
    }
    return err;
}

std::string CheckRepeatUntil(Deque*& deque, std::string oper, const int depth) {
    std::string err;
    if (oper == "REPEAT" || oper == "UNTIL") {
        if (depth < 1) {
            err = "error nesting " + oper;
        }
    }
    return err;
}

std::string Analize(Deque* deque) {
    int depth = 0;
    std::string err;
    while (deque->len() > 0 && err.empty() && depth >= 0) {
        std::string oper = GetOperand(deque);
        err = CheckRecord(deque, oper, depth);
        if (err.empty()) err = CheckCase(deque, oper, depth);
        if (err.empty()) err = CheckRepeatUntil(deque, oper, depth);
        if (oper == "BEGIN") depth++;
        if (oper == "END") depth--;
    }

    if (depth != 0 && err.empty()) {
        err = "error nesting";
    }

    return err;
}

int main() {
    Deque* deque = new Deque();
    std::ifstream infile;
    infile.open("/home/bomber-hol/Projects/Algos/lab_2/num_25/input.txt");
    if (infile.is_open()) {
        FillDeque(infile, deque);
        std::string err = Analize(deque);
        if (err != "") {
            std::cerr << err << std::endl;
            return 1;
        } else {
            std::cout << "It`s OK!" << std::endl;
        }
    } else {
        std::cerr << "Error opening file" << std::endl;
    }
}