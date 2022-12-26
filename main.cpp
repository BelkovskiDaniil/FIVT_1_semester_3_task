/* Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
 * Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
 * Добавление узла в декартово дерево выполняйте следующим образом:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве все ключи меньше x, а во втором больше или равны x.
 * Получившиеся два дерева сделайте дочерними для нового узла (x, y). Новый узел вставьте на место узла P.
 * Построить также наивное дерево поиска по ключам Xi.
 * Т.е., при добавлении очередного числа K в наивное дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Вычислить разницу глубин наивного дерева поиска и декартового дерева.
 * Разница может быть отрицательна, необходимо вывести модуль разности.*/

//https://contest.yandex.ru/contest/43508/run-report/80024323/

#include <iostream>
#include <vector>

struct Node {
    ~Node();

    int Data;
    Node* Left = nullptr;
    Node* Right = nullptr;
    Node* Parent = nullptr;

    explicit Node(int data, Node* parent = nullptr) : Data(data), Parent(parent) {}
};

Node::~Node() {
    delete Left;
    delete Right;
}

class Tree {
public:
    ~Tree();
    void Add(int key);
    // Возвращает true, если успешно удалили
    int GetDepth() const;
    void Print() const;

private:
    Node* root = nullptr;

    int GetDepth(Node* node) const;
    void Print(Node* node, int tabs) const;
};

Tree::~Tree() {
    delete root;
}

void Tree::Add(int key) {
    if (!root) {
        root = new Node(key);
        return;
    }
    Node* current = root;
    while (true) {
        if (current->Data > key) {
            // Идем в Left
            if (current->Left != nullptr)
                current = current->Left;
            else {
                current->Left = new Node(key, current);
                break;
            }
        } else {
            // Идем в Right
            if (current->Right != nullptr)
                current = current->Right;
            else {
                current->Right = new Node(key, current);
                break;
            }
        }
    }
}

//Получаем глубину дерева
int Tree::GetDepth() const {
    return GetDepth(root);
}

//Получаем глубину дерева
int Tree::GetDepth(Node *node) const {
    if (!node) return 0;
    return std::max(GetDepth(node->Left), GetDepth(node->Right)) + 1;
}

void Tree::Print() const {
    if (root) Print(root, 0);
}

void Tree::Print(Node* node, int tabs) const {
    if (node->Right) Print(node->Right, tabs + 1);
    for (int i = 0; i < tabs; ++i) std::cout << "\t";
    std::cout << node->Data << std::endl;
    if (node->Left) Print(node->Left, tabs + 1);
}

struct TreapNode {
    int Key;
    int Priority;
    TreapNode* Left = nullptr;
    TreapNode* Right = nullptr;

    TreapNode(int key, int priority) : Key(key), Priority(priority) {}
};

class Treap {
public:
    void Add(int key, int prioryty);
    void Print() const { if (root) Print(root, 0); }
    int GetDepth() const;

private:
    TreapNode* root = nullptr;
    std::pair<TreapNode*, TreapNode*> Split(int key, TreapNode* node);
    TreapNode* Merge(TreapNode* t1, TreapNode* t2);

    int GetDepth(TreapNode* node) const;
    void Print(TreapNode* node, int tabs) const;
};

//Сплит дерева
std::pair<TreapNode*, TreapNode*> Treap::Split(int key, TreapNode* node) {
    if (!node) return {nullptr, nullptr};
    if (node->Key < key) {
        auto [t1, t2] = Split(key, node->Right);
        node->Right = t1;
        return {node, t2};
    } else {
        auto [t1, t2] = Split(key, node->Left);
        node->Left = t2;
        return {t1, node};
    }
}

//Функция слияния
TreapNode* Treap::Merge(TreapNode* left, TreapNode* right) {
    if (left == nullptr || right == nullptr) {
        return left == nullptr ? right : left;
    }
    if (left->Priority > right->Priority) {
        left->Right = Merge(left->Right, right);
        return left;
    } else {
        right->Left = Merge(left, right->Left);
        return right;
    }
}

//Добавляем и устанавливаем, исходя из приоритета
void Treap::Add(int key, int prioryty) {
    auto [t1, t2] = Split(key, root);
    int r = prioryty;
    TreapNode* new_node = new TreapNode(key, r);
    t1 = Merge(t1, new_node);
    root = Merge(t1, t2);
}

//Получаем глубину дерева
int Treap::GetDepth() const {
    return GetDepth(root);
}

//Получаем глубину дерева
int Treap::GetDepth(TreapNode *node) const {
    if (!node) return 0;
    return std::max(GetDepth(node->Left), GetDepth(node->Right)) + 1;
}

void Treap::Print(TreapNode* node, int tabs) const {
    if (node->Right) Print(node->Right, tabs + 1);
    for (int i = 0; i < tabs; ++i) std::cout << "\t";
    std::cout << node->Key << std::endl;
    if (node->Left) Print(node->Left, tabs + 1);
}

int main() {
    Treap treap;
    Tree tree;
    int n = 0, a = 0, b = 0;
    std::cin >> n;
    for(int i = 0; i < n; i++){
        std::cin >> a >> b;
        tree.Add(a);
        treap.Add(a, b);
    }

    std::cout << abs(tree.GetDepth() - treap.GetDepth());
    return 0;
}
