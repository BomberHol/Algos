#include <iostream>



class Node {
public:
    std::string word;
    Node* next;
    Node(std::string inword) {
        word = inword;
        next = nullptr;
    }
};

class Stack {
private:
    Node* first;
    // Node* end;

    void InsertNodeInEnd(Node*& node, std::string inword) {
        if (node == nullptr) {
            node = new Node(inword);
        } else {
            InsertNodeInEnd(node->next, inword);
        }
    }

public:


    Stack() {

    }

    void add(std::string inword) {
        if (first == nullptr) {
            first = new Node(inword);
        } else {
            InsertNodeInEnd(first, inword);
        }
    }

    int len() {
        int count = 0;
        Node*& node1 = first;
        Node* node2 = first;
        // while (stack != nullptr) {
        //     count++;
        //     stack = stack->next;
        // }
        return count;
    }
};

int main() {
    Stack* stack = new Stack();
    stack->add("Hello");
    stack->add("Goodbye");

    // std::cout << stack->word << " " << ;
}