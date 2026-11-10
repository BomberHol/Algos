#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
using namespace std;

constexpr char AND_NODE = '&';
constexpr char OR_NODE = '|';
constexpr char LIST_NODE = '=';
constexpr char NULL_NODE = '~';
constexpr int INIT_PRICE = -1;
constexpr int ERROR_INDEX = -2;
constexpr int NULL_VALUE_NODE = -3;
constexpr int ERROR_VALUE = -4;
constexpr int NULL_NUMBER_VISIT = -5;
constexpr int INFINITY_NUMBER_VISIT = -6;

class Node {
public:
    virtual ~Node() {
        for (Node* child: nodes) {
            delete child;
        }
        nodes.clear();
    }
    virtual string getType() = 0;
    virtual int getCountChild() = 0;

protected:
    string data;
    int price = INIT_PRICE;
    vector<Node*> nodes;
    int maxNumberOfVisit = NULL_NUMBER_VISIT;
    int numberOfVisit = 0;
    int count = 1;
public:
    void addChild(Node *node) {
        nodes.push_back(node);
    }
    void removeChildAt(Node *node) {
        const size_t index = getIndexChildOf(node);
        if (index >= nodes.size()) return;
        delete nodes[index];
        nodes.erase(nodes.begin() + index);
    }
    int getPrice() {
        return price;
    }
    string getData() {
        return data;
    }
    vector<Node*> getChildren() {
        return nodes;
    }
    int getIndexChildOf(const Node *node) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i] == node) {
                return i;
            }
        }
        return ERROR_INDEX;
    }
    int getMaxNumberOfVisit() {
        return maxNumberOfVisit;
    }
    void setMaxNumberOfVisit(int inMaxNumberOfVisit) {
        maxNumberOfVisit = inMaxNumberOfVisit;
    }
    void incNumberOfVisit() {
        numberOfVisit++;
    }
    int getCount() {
        return count;
    }
    void setCount(int inCount) {
        count = inCount;
    }
};

class AndNode: public Node {
public:
    AndNode(const string &inData) {
        data = inData;
    }
    string getType() override {
        return "AND";
    }
    int getCountChild() override {
        int count = 1;
        for (int i = 0; i < nodes.size(); i++) {
            count *= nodes[i]->getCount();
        }
        return count;
    }
};

class OrNode: public Node {
public:
    OrNode(const string &inData) {
        data = inData;
    }
    string getType() override {
        return "OR";
    }
    int getCountChild() override {
        int count = 0;
        for (int i = 0; i < nodes.size(); i++) {
            count += nodes[i]->getCount();
        }
        return count;
    }
};

class ListNode: public Node {
public:
    ListNode(const string inData, const int inPrice) {
        data = inData;
        price = inPrice;
    }
    string getType() override {
        return "LIST";
    }
    int getCountChild() override {
        return 1;
    }
};


//----------------------NODE------------------------------


class Tree {
private:
    string name;
    Node* root;
    stack<Node*> Stack;
    vector<Node*> variants;

    void fillTreeForStack(int countDot, char type, string data, int value) {
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
            if (!Stack.empty() && Stack.top()->getType() == "LIST") {
                throw runtime_error("Дерево построено неправильно. У листа не может быть дочерных объектов!");
            }
            Node* newNode = nullptr;
            if (type == AND_NODE) {
                newNode = new AndNode(data);
            } else if (type == OR_NODE) {
                newNode = new OrNode(data);
            } else {
                newNode = new ListNode(data, value);
            }

            if (root == nullptr) {
                root = newNode;
                Stack.push(newNode);
            } else {
                Node* topNode = Stack.top();
                topNode->addChild(newNode);
                Stack.push(newNode);
            }
        }
    }
    void clearStack() {
        while (!Stack.empty()) {
            Stack.pop();
        }
    }
    void pushAndPrintNode(Node* node) {
        Stack.push(node);
        int depth = Stack.size() - 1;
        for (int i = 0; i < depth; i++) {
            cout << "    ";
        }
        cout << "── " << node->getData();
        cout << "(" << node->getCount() << ")";
        cout << "[" << node->getType() << "]";
        cout << endl;
    }
    int getNumFromFile(ifstream& inFile) {
        char ch;
        string numStr;
        inFile.get(ch);

        if ((ch == '-') || (ch >= '0' && ch <= '9')) {
            numStr += ch;
        }
        while (inFile.get(ch) && ch != '\n') {
            if (ch >= '0' && ch <= '9') {
                numStr += ch;
            } else {
                throw runtime_error("Ошибка значения листа");
            }
        }
        return stoi(numStr);
    }

public:
    Tree(string inName) {
        root = nullptr;
        name = inName;
    }

    void readTreeFromFile(ifstream& inFile) {
        int countDot = 0;
        int value;
        string data;
        char type;
        char ch;

        while (!inFile.eof()) {
            countDot = 0;
            data = "";
            type = NULL_NODE;
            value = NULL_VALUE_NODE;
            while (inFile.get(ch) && ch != '\n') {
                if (ch == '.') {
                    countDot++;
                } else if (ch == AND_NODE || ch == OR_NODE) {
                    type = ch == OR_NODE ? OR_NODE : AND_NODE;
                } else if (ch == '=') {
                    value = getNumFromFile(inFile);
                    type = LIST_NODE;
                    break;
                } else {
                    data += ch;
                }
            }
            // cout << countDot << " " << type << " " << data << endl;
            fillTreeForStack(countDot, type, data, value);
        }
        cout << endl;
        clearStack();
    }
    void writeTree() {
        if (root == nullptr) {
            throw runtime_error("Дерево пустое");
        }

        Node* deleteNode = nullptr;
        pushAndPrintNode(root);

        while (!Stack.empty()) {
            Node* topNode = Stack.top();
            vector<Node*> children = topNode->getChildren();
            if (!children.empty()) {
                int index = topNode->getIndexChildOf(deleteNode);
                if (index == ERROR_INDEX) {
                    pushAndPrintNode(children[0]);
                } else {
                    if (index == children.size() - 1) {
                        deleteNode = topNode;
                        Stack.pop();
                    } else {
                        pushAndPrintNode(children[index + 1]);
                    }
                }
            } else {
                deleteNode = topNode;
                Stack.pop();
            }
        }
        clearStack();
    }

    void calculateMaxVisit() {
        if (root == nullptr) {
            throw runtime_error("Дерево пустое");
        }

        int countTree = 0;
        Node* deleteNode = nullptr;
        Stack.push(root);
        while (!Stack.empty()) {
            Node* topNode = Stack.top();
            vector<Node*> children = topNode->getChildren();

            if (!children.empty()) {
                int index = topNode->getIndexChildOf(deleteNode);
                if (index == ERROR_INDEX) {
                    Stack.push(children[0]);
                } else {
                    if (index == children.size() - 1) {  // прошел по всем детям
                        deleteNode = topNode;
                        Stack.pop();
                        if (deleteNode->getType() == "AND" || deleteNode->getType() == "OR") {
                            deleteNode->setCount(deleteNode->getCountChild());
                        }
                    } else {
                        Stack.push(children[index + 1]);
                    }
                }
            } else {
                deleteNode = topNode;
                Stack.pop();
            }
        }
        cout << countTree << endl;
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
        tree->writeTree();
        tree->calculateMaxVisit();
        tree->writeTree();
    } catch (runtime_error& e) {
        cerr << e.what() << endl;
    }

}