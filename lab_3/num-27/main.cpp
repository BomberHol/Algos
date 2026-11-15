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
    virtual int getCountVariantsForChild() = 0;

protected:
    string data;
    int price = INIT_PRICE;
    vector<Node*> nodes;
    int maxNumberOfVisit = NULL_NUMBER_VISIT;
    int numberOfVisit = 0;
    int countVariants = 1;
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
    int getNumberOfVisit() {
        return numberOfVisit;
    }
    int getCountVariants() {
        return countVariants;
    }
    void setCountVariants(int inCount) {
        countVariants = inCount;
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
    int getCountVariantsForChild() override {
        int count = 1;
        for (int i = 0; i < nodes.size(); i++) {
            count *= nodes[i]->getCountVariants();
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
    int getCountVariantsForChild() override {
        int count = 0;
        for (int i = 0; i < nodes.size(); i++) {
            count += nodes[i]->getCountVariants();
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
    int getCountVariantsForChild() override {
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
        cout << "(" << node->getCountVariants() << ")";
        cout << "[" << node->getType() << "]";
        cout << node->getMaxNumberOfVisit();
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

    void calculateAllElementsTree() {
        if (root == nullptr) {
            throw runtime_error("Дерево пустое");
        }

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
                            deleteNode->setCountVariants(deleteNode->getCountVariantsForChild());
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
        clearStack();
    }

    void calculateMaxVisit() {
        if (root == nullptr) {
            throw runtime_error("Дерево пустое");
        }

        Node* deleteNode = nullptr;
        Stack.push(root);
        root->setMaxNumberOfVisit(root->getCountVariantsForChild());
        while (!Stack.empty()) {
            Node* topNode = Stack.top();
            vector<Node*> children = topNode->getChildren();

            if (!children.empty()) {
                int index = topNode->getIndexChildOf(deleteNode);
                if (index == ERROR_INDEX) {
                    Stack.push(children[0]);
                    if (topNode->getType() == "AND") {
                        children[0]->setMaxNumberOfVisit(topNode->getMaxNumberOfVisit());
                    } else if (topNode->getType() == "OR") {
                        children[0]->setMaxNumberOfVisit((topNode->getMaxNumberOfVisit() / topNode->getCountVariantsForChild()) * children[0]->getCountVariants());
                    }

                } else {
                    if (index == children.size() - 1) {  // прошел по всем детям
                        deleteNode = topNode;
                        Stack.pop();
                        if (deleteNode->getType() == "AND" || deleteNode->getType() == "OR") {
                            deleteNode->setCountVariants(deleteNode->getCountVariantsForChild());
                        }
                    } else {
                        Stack.push(children[index + 1]);
                        if (topNode->getType() == "AND") {
                            children[index + 1]->setMaxNumberOfVisit(topNode->getMaxNumberOfVisit());
                        } else if (topNode->getType() == "OR") {
                            children[index + 1]->setMaxNumberOfVisit((topNode->getMaxNumberOfVisit() / topNode->getCountVariantsForChild()) * children[index + 1]->getCountVariants());
                        }
                    }
                }
            } else {
                deleteNode = topNode;
                Stack.pop();
            }
        }
        clearStack();
    }

    void write() {
        cout << root->getCountVariants() << endl;
    }

    void writeElement(Node* node) {
        int depth = Stack.size() - 1;
        for (int i = 0; i < depth; i++) {
            cout << "    ";
        }
        cout << "── " << node->getData();
        cout << endl;
    }

    void pushAndInc(Node* node) {
        Stack.push(node);
        node->incNumberOfVisit();
    }

    int getRoundOff(int countOfVisit, int countChild) {
        while (countOfVisit >= countChild) {
            countOfVisit -= countChild;
        }
        return countOfVisit;
    }

    void writeAllElements() {
        if (root == nullptr) {
            throw runtime_error("Дерево пустое");
        }

        struct StackEntity {
            Node* node;
            int variant;
            int depth;
        };

        stack<StackEntity> Stack;
        const int totalVariants = root->getCountVariants();
        for (int variant = 0; variant < totalVariants; variant++) {
            Stack.push({root, variant, 0});
            while (!Stack.empty()) {
                StackEntity top = Stack.top();
                Stack.pop();
                top.node->incNumberOfVisit();
                for (int depth = 0; depth < top.depth; depth++) {
                    cout << "    ";
                }
                cout << "── " << top.node->getData() << '\n';
                vector<Node*> children = top.node->getChildren();
                int localVariant = top.variant;
                if (top.node->getType() == "AND") {
                    for (int i = 0; i < children.size(); i++) {
                        Node* child = children[i];
                        int countVariantsChild = child->getCountVariants();
                        Stack.push({child, localVariant % countVariantsChild, top.depth + 1});
                        localVariant /= countVariantsChild;
                    }
                } else if (top.node->getType() == "OR") {
                    for (int i = 0; i < children.size(); i++) {
                        int countVariantsChild = children[i]->getCountVariants();
                        if (localVariant < countVariantsChild) {
                            Stack.push({children[i], localVariant, top.depth + 1});
                            break;
                        }
                        localVariant -= countVariantsChild;
                    }
                }
            }
        }
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
        tree->calculateAllElementsTree();
        tree->calculateMaxVisit();
        tree->writeTree();
        cout << endl << endl << endl;
        tree->write();
        tree->writeAllElements();
    } catch (runtime_error& e) {
        cerr << e.what() << endl;
    }

}