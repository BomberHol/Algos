// Антнов Артём Сергеевич
// ПС-21
// Лабараторная работа номер 2
//
// Задание 25
// Программа  на ПАСКАЛЕ включает такие сочетания ключевых
// слов, как  REPEAT..UNTIL, RECORD..END, CASE..END и BEGIN..END.
// Некоторые части программы могут быть  закомментированы, другие
// части текста могут представлять собой  константы в апострофах.
// Требуется проверить правильность вложенности этих  конструкций
// с  учетом  допустимости  взаимных  вложений.  В случае  ошибок
// указать номер первой некорректной строки (11).
//
// Среда выполнения - CLion GetBrains

#include <fstream>
#include <iostream>
#include <limits>

const std::string NULL_STR = "";

class Data {
private:
    std::string word;
    int x;
    int y;
public:
    Data(const std::string& inWord, const int inX, const int inY) {
        word = inWord;
        x = inX;
        y = inY;
    }

    std::string getWord() {
        return word;
    }

    int getX() {
        return x;
    }

    int getY() {
        return y;
    }
};

class Node {
private:
    Data* data;
    Node* prev;
public:
    Node(const std::string& inWord, const int inX, const int inY) {
        data = new Data(inWord, inX, inY);
        prev = nullptr;
    }

    ~Node() {
        delete data;
    }

    void setPoint(Node*& inPtr) {
        prev = inPtr;
    }

    Node* getPrev() {
        return prev;
    }

    Data* getData() {
        return data;
    }
};

class Stack {
private:
    int len;
    Node* top;


public:
    Stack() {
        top = nullptr;
        len = 0;
    }

    ~Stack() {
        while (top != nullptr) {
            Node* prev = top->getPrev();
            delete top;
            top = prev;
        }
    }

    void push(const std::string& inWord, const int inX, const int inY) {
        if (top == nullptr) {
            top = new Node(inWord, inX, inY);
            len++;
        } else {
            Node* newNode = new Node(inWord, inX, inY);
            newNode->setPoint(top);
            top = newNode;
            len++;
        }
    }

    void pop() {
        if (top != nullptr) {
            Node* prevNode = top->getPrev();
            delete top;
            top = prevNode;
            len--;
        }
    }

    Data* getTop() {
        if (top != nullptr) {
            return top->getData();
        }
        return nullptr;
    }

    int getLen() {
        return len;
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

std::string InsertOper(Stack* stack, std::string oper, const int inX, const int inY) {
    std::string err;

    if (stack->getLen() == 0) {
        if (oper == "BEGIN" || oper == "RECORD") {
            stack->push(oper, inX, inY);
        } else {
            err = "1. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
        }
        return err;
    }

    std::string top = stack->getTop()->getWord();

    if (oper == "BEGIN" && top != "CASE" && top != "RECORD") {
        stack->push(oper, inX, inY);
    } else if (oper == "BEGIN" && (top == "CASE" || top == "RECORD")) {
        return "2. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
    }

    if ((oper == "REPEAT" || oper == "CASE") && (top == "BEGIN" || top == "REPEAT")) {
        stack->push(oper, inX, inY);
    } else if ((oper == "REPEAT" || oper == "CASE") && top != "BEGIN" && top != "REPEAT") {
        return "3. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
    }

    if (oper == "RECORD" && stack->getLen() == 0) {
        stack->push(oper, inX, inY);
    } else if (oper == "RECORD" && stack->getLen() != 0) {
        return "4. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
    }

    if (oper == "END" && top != "REPEAT") {
        stack->pop();
    } else if (oper == "END" && top == "REPEAT") {
        return "5. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
    }

    if (oper == "UNTIL" && top == "REPEAT") {
        stack->pop();
    } else if (oper == "UNTIL" && top != "REPEAT") {
        return "6. error by " + oper + " строка: " + std::to_string(inY) + " символ: " + std::to_string(inX);
    }

    return err;
}

void SkipSymbols(std::ifstream& infile, int& x, int& y, char to) {
    char ch;
    while (infile.get(ch) && ch != to) {
        x++;
        if (ch == '\n') {
            y++;
            x = 0;
        }
    }
}

std::string FillStack(std::ifstream& infile, Stack*& stack) {
    char ch;
    std::string word;
    int x = 1;
    int y = 1;
    std::string err;
    while (infile.get(ch) && err.empty()) {
        if (ch == '/') {
            infile.get(ch);
            if (ch == '/') {
                SkipSymbols(infile, x, y, '\n');
                y++;
                x += infile.gcount();
                continue;
            }
        }

        if (ch == '\047') {
            SkipSymbols(infile, x, y, '\047');
            continue;
        }

        if (ch == '{') {
            SkipSymbols(infile, x, y, '}');
            continue;
        }

        if (!IsLetter(ch) && !word.empty()) {
            word = GetUppercaseStr(word);
            if (word == "BEGIN" || word == "END" || word == "REPEAT" || word == "UNTIL" || word == "CASE" || word == "RECORD") {
                err = InsertOper(stack, word, x, y);
            }
            word = "";
        } else if (IsLetter(ch)) {
            word += ch;
        }
        x++;
        if (ch == '\n') {
            y++;
            x = 0;
        }
    }

    if (!word.empty()) { // нужно вынести
        word = GetUppercaseStr(word);
        if (word == "BEGIN" || word == "END" || word == "RECORD" || word == "CASE" || word == "REPEAT" || word == "UNTIL") {
            err = InsertOper(stack, word, x, y);
        }
        word = "";
    }

    if (stack->getLen() != 0 && err.empty()) {
        err = "error by close operator строка: " + std::to_string(y) + " символ: " + std::to_string(x);
    }
    return err;
}

int main() {
    Stack* stack = new Stack();
    std::ifstream infile;
    infile.open("/home/bomber-hol/Projects/Algos/lab_2/num_25/input.txt");
    if (infile.is_open()) {
        std::string err = FillStack(infile, stack);
        if (err != "") {
            std::cerr << err << std::endl;
            return 1;
        }
        std::cout << "It`s OK!" << std::endl;
    } else {
        std::cerr << "Error opening file" << std::endl;
    }
}