#include <iostream>
#include <vector>
using namespace std;

const string AND_NODE = "and";
const string OR_NODE = "or";
const int INIT_PRICE = -1;
const int ERROR_INDEX = -2;

class Node {
public:
    virtual ~Node() {
        for (Node* child: nodes) {
            delete child;
        }
        nodes.clear();
    }
    virtual void calculateMinCost();
    virtual void calculateMaxCost();

protected:
    string data;
    int price = INIT_PRICE;
    vector<Node*> nodes;
    void addNode(Node *node) {
        nodes.push_back(node);
    }
    void removeNodeAt(const int index) {
        if (index >= nodes.size()) return;
        delete nodes[index];
        nodes.erase(nodes.begin() + index);
    }
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

    }
    void calculateMaxCost() override {

    }
};

class OrNode: public Node {
public:
    OrNode(const string &inData) {
        data = inData;
    }
    void calculateMinCost() override {

    }
    void calculateMaxCost() override {

    }
};





class Tree {
public:
    string name;
    Node *Root = nullptr;

    void add(const string &type, const string &data, const int prior) {

    }
};


int main() {
    Node* node = new AndNode("123");
}