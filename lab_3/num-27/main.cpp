#include <iostream>
#include <vector>
using namespace std;

constexpr string AND_NODE = "and";
constexpr string OR_NODE = "or";
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
    void addNode(Node *&node) {
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





class Tree {
public:
    string name;
    Node *Root = nullptr;

    void add(const string &type, const string &data, const int prior) {

    }
};


int main() {
    Node* node = new AndNode("Dad");
    Node* newNode1 = new OrNode("son1");

    node->addNode(newNode1);

    newNode1->calculateMinCost();
    node->calculateMinCost();
    cout << node->getPrice() << endl;
}