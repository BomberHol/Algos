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
        // clear();
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

    int len() {
        int count = 0;
        Node* node = beg;
        while (node != nullptr) {
            count++;
            node = node->next;
        }
        return count;
    }
};

int main() {
    Deque* deque = new Deque();
    deque->push_front("0");  // 0
    deque->push_front("+1");  // +1 0
    deque->push_back("-1");  // +1 0 -1

    deque->pop_back();
}