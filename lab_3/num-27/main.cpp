#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

constexpr char AND_NODE = '&';
constexpr char OR_NODE = '|';
constexpr char NULL_NODE = '~';
constexpr int INIT_PRICE = -1;
constexpr int ERROR_INDEX = -2;

class Node {
public:
    virtual ~Node() {
        for (Node* child: nodes) {
            delete child;
        }
        nodes.clear();
    }
    virtual void calculateMinCost() = 0;
    virtual void calculateMaxCost() = 0;

protected:
    string data;
    int price = INIT_PRICE;
    vector<Node*> nodes;
public:
    void addNode(Node *node) {
        nodes.push_back(node);
    }
    void removeChildAt(Node *node) {
        const size_t index = IndexOf(node);
        if (index >= nodes.size()) return;
        delete nodes[index];
        nodes.erase(nodes.begin() + index);
    }
    int getPrice() {
        return price;
    }
private:
    int IndexOf(const Node *node) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i] == node) {
                return i;
            }
        }
        return ERROR_INDEX;
    }

};

class AndNode: public Node {
public:
    AndNode(const string &inData) {
        data = inData;
    }
    void calculateMinCost() override {
        price = 0;
        for (int i = 0; i < nodes.size(); i++) {
            price += nodes[i]->getPrice();
        }
    }
    void calculateMaxCost() override {
        price = 0;
        for (int i = 0; i < nodes.size(); i++) {
            price += nodes[i]->getPrice();
        }
    }
};

class OrNode: public Node {
public:
    OrNode(const string &inData) {
        data = inData;
    }
    void calculateMinCost() override {
        price = 0;
        for (int i = 0; i < nodes.size(); i++) {
            int childPrice = nodes[i]->getPrice();
            if (price > childPrice) {
                price = childPrice;
            }
        }
    }
    void calculateMaxCost() override {
        price = 0;
        for (int i = 0; i < nodes.size(); i++) {
            int childPrice = nodes[i]->getPrice();
            if (price < childPrice) {
                price = childPrice;
            }
        }
    }
};

//----------------------NODE------------------------------


class Tree {
private:
    string name;
    Node* root;
    stack<Node*> Stack;
    void fillTreeForStack(int countDot, char type, string data) {
        if (type == NULL_NODE) {
            throw runtime_error("Не указан тип узла в файле!");
        }

        if (countDot > Stack.size()) {
            throw runtime_error("Ошибка записи дерева в файле. countDot > Stack.size()");
        }

        if (countDot < Stack.size()) {
            int delta = Stack.size() - countDot;
            while (delta > 0) {
                Stack.pop();
                delta--;
            }
            if (Stack.empty()) {
                throw runtime_error("Ошибка записи дерева в файле. Повторное добавление корня дерева в файл.");
            }
        }

        if (countDot == Stack.size()) {
            Node* newNode = nullptr;
            if (type == AND_NODE) {
                newNode = new AndNode(data);
            } else {
                newNode = new OrNode(data);
            }

            if (root == nullptr) {
                root = newNode;
                Stack.push(newNode);
            } else {
                Node* topNode = Stack.top();
                topNode->addNode(newNode);
                Stack.push(newNode);
            }
        }
    }
    void clearStack() {
        while (!Stack.empty()) {
            Stack.pop();
        }
    }

public:
    Tree(string inName) {
        root = nullptr;
        name = inName;
    }

    void readTreeFromFile(ifstream& inFile) {
        int countDot = 0;
        string data;
        char type;
        char ch;

        while (!inFile.eof()) {
            countDot = 0;
            data = "";
            type = NULL_NODE;
            while (inFile.get(ch) && ch != '\n') {
                if (ch == '.') {
                    countDot++;
                } else if (ch == AND_NODE || ch == OR_NODE) {
                    type = ch == OR_NODE ? OR_NODE : AND_NODE;
                } else {
                    data += ch;
                }
            }
            cout << countDot << " " << type << " " << data << endl;
            fillTreeForStack(countDot, type, data);
        }
        cout << endl;
        clearStack();
    }
    void writeTree() {

    }
    void writeAllElements() {

    }
    void calculateMinTreeCost() {

    }
    void calculateMaxTreeCost() {

    }
};


int main() {
    Tree* tree = new Tree("My Tree");
    ifstream inFile = ifstream("in.txt");
    inFile.open("/home/bomber-hol/Public/Algos/lab_3/num-27/in.txt");

    try {
        if (!inFile.is_open()) {
            throw runtime_error("Ошибка открытия файла!");
        }
        tree->readTreeFromFile(inFile);
    } catch (runtime_error& e) {
        cerr << e.what() << endl;
    }


}