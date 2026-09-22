#include <deque>
#include <fstream>
#include <iostream>

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

int main() {
    // Deque* deque = new Deque();
    // std::ifstream infile;
    // infile.open("/home/bomber-hol/Projects/Algos/lab_2/num_25/input.txt");
    // if (infile.is_open()) {
    //     FillDeque(infile, deque);
    //     std::string err = Analize(deque);
    //     if (err != "") {
    //         std::cerr << err << std::endl;
    //         return 1;
    //     } else {
    //         std::cout << "It`s OK!" << std::endl;
    //     }
    // } else {
    //     std::cerr << "Error opening file" << std::endl;
    // }

    Stack* stack = new Stack();
    stack->push("1", 1, 1);
    stack->push("2", 2, 2);
    stack->pop();
    Data* data = stack->getTop();
    std::cout << data->getWord() << " " << data->getX() << " " << data->getY() << std::endl;
}