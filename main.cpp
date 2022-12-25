/*Дано число N и N строк. Каждая строка содержащит команду добавления или удаления натуральных чисел,
 * а также запрос на получение k-ой порядковой статистики. Команда добавления числа
 * A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требуемая скорость выполнения запроса - O(log n).*/

//https://contest.yandex.ru/contest/43508/run-report/80034640/

#include <iostream>

//Структура узла
struct avl_node {
    explicit avl_node(const int &data) : data(data) {};
    ~avl_node();
    int data;
    int height = 1;
    int count = 1;
    avl_node *left = nullptr;
    avl_node *right = nullptr;
};

avl_node::~avl_node() {
    delete left;
    delete right;
}

int get_height(const avl_node *node) {
    return node ? node->height : 0;
}

int get_count(const avl_node *node) {
    return node ? node->count : 0;
}

void update_height(avl_node *node) {
    node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
}

void update_count(avl_node *node) {
    node->count = get_count(node->left) + get_count(node->right) + 1;
}

int balance(const avl_node *node) {
    return (get_height(node->right) - get_height(node->left));
}

//Структура дерева
class avl_tree {
public:

    explicit avl_tree(avl_node *root = nullptr) : root(root) {};
    ~avl_tree();
    void add(const int &data);
    void del(const int &data);
    int k_stat(const int &k);
    void print() const { print(root, 0); };

private:
    avl_node *root = nullptr;
    avl_node *add(avl_node *node, const int &data);
    avl_node *del(avl_node *node, const int &data);
    static avl_node *rotate_left(avl_node *node);
    static avl_node *rotate_right(avl_node *node);
    static avl_node *fix_balance(avl_node *node);
    int k_stat(const int &k, const avl_node *node);
    static void print(avl_node* node, int tabs);

};

void avl_tree::add(const int &value) {
    root = add(root, value);
}

void avl_tree::del(const int &value) {
    root = del(root, value);
}

int avl_tree::k_stat(const int &k) {
    return k_stat(k, root);
}

avl_tree::~avl_tree() {
    delete root;
}

avl_node *avl_tree::add(avl_node *node, const int &data) {
    if (!node) {
        return new avl_node(data);
    } else if (data < node->data) {
        node->left = add(node->left, data);
    } else {
        node->right = add(node->right, data);
    }
    update_height(node);
    update_count(node);
    return fix_balance(node);
}

avl_node *avl_tree::del(avl_node *node, const int &data) {
    if (!node) {
        return nullptr;
    }

    if (data < node->data) {
        node->left = del(node->left, data);
    } else if (data > node->data) {
        node->right = del(node->right, data);
    } else {
        if (node->left == nullptr || node->right == nullptr) {
            avl_node *temp = node->left ? node->left : node->right;
            if (!temp) {
                temp = node;
                node = nullptr;
            }
            else {
                *node = *temp;
            }
            delete temp;
        }
        else {
            avl_node *temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->data = temp->data;
            node->right = del(node->right, temp->data);
        }
    }
    if (!node) {
        return nullptr;
    }
    update_height(node);
    update_count(node);
    return fix_balance(node);
}

//Левый поворот
avl_node *avl_tree::rotate_left(avl_node *node) {
    avl_node *temp = node->right;
    node->right = temp->left;
    temp->left = node;

    update_height(node);
    update_count(node);
    update_height(temp);
    update_count(temp);
    return temp;
}

//Правый поворот
avl_node *avl_tree::rotate_right(avl_node *node) {
    avl_node *left(node->left);
    node->left = left->right;
    left->right = node;

    update_height(node);
    update_count(node);
    update_height(left);
    update_count(left);
    return left;
}

//Восстановление баланса
avl_node *avl_tree::fix_balance(avl_node *node) {
    if (balance(node) == 2) {
        if (balance(node->right) == -1) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    if (balance(node) == -2) {
        if (balance(node->left) == 1) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    return node;
}

//Поиск статистики
int avl_tree::k_stat(const int &k, const avl_node *node) {
    if (get_count(node->left) == k) {
        return node->data;
    }
    if (get_count(node->left) > k) {
        return k_stat(k, node->left);
    }
    return k_stat(k - get_count(node->left) - 1, node->right);
}

//Печать
void avl_tree::print(avl_node* node, int tabs) {
    if (node->right) print(node->right, tabs + 1);
    for (int i = 0; i < tabs; ++i) std::cout << "\t";
    std::cout << node->data << "(" << node->count << ")" << std::endl;
    if (node->left) print(node->left, tabs + 1);
}

int main() {
    avl_tree tree;
    int n = 0, a = 0, b = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> a >> b;
        if (a < 0) {
            tree.del(abs(a));
        }
        else {
            tree.add(a);
        }
        std::cout << tree.k_stat(b) << " ";
    }
    std::cout << std::endl;

    return 0;
}
