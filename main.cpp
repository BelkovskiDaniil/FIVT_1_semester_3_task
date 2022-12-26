/*Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево, заданное наивным порядком вставки.
 * Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
 * то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 * Выведите элементы в порядке pre-order (сверху вниз).
 * Рекурсия запрещена. */

//https://contest.yandex.ru/contest/43508/run-report/80023763/

#include <iostream>
#include <vector>
#include <queue>

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
    void add(int key);
    std::deque<int> pre_order();

private:
    Node* root = nullptr;
};

Tree::~Tree() {
    delete root;
}

void Tree::add(int key) {
    if (!root) {
        root = new Node(key);
        return;
    }

    Node* current = root;
    while (true) {

        if (current->Data > key) {
            if (current->Left != nullptr)
                current = current->Left;
            else {
                current->Left = new Node(key, current);
                break;
            }
        }

        else {
            if (current->Right != nullptr)
                current = current->Right;
            else {
                current->Right = new Node(key, current);
                break;
            }
        }
    }
}

std::deque<int> Tree::pre_order() {
    std::deque<int> answer;

    if (root == nullptr){
        return answer;
    }

    std::deque<Node*> queue_local_left;
    std::deque<Node*> queue_local_right;
    queue_local_left.push_back(root);
    while((!queue_local_left.empty()) or (!queue_local_right.empty())){

        //Сначала проверяем левую ветвь на пустоту
        if (!queue_local_left.empty()) {
            Node * node = queue_local_left.front();
            queue_local_left.pop_front();
            answer.push_back(node->Data);
            //Выбираем дальнейший путь
            if(node->Left != nullptr){
                queue_local_left.push_front(node->Left);
            }

            if(node->Right != nullptr){
                queue_local_right.push_front(node->Right);
            }
        }

        //Затем правую
        else if (!queue_local_right.empty()) {
            Node * node = queue_local_right.front();
            queue_local_right.pop_front();
            answer.push_back(node->Data);
            //Выбираем дальнейший путь
            if(node->Left != nullptr){
                queue_local_left.push_front(node->Left);
            }

            if(node->Right != nullptr){
                queue_local_right.push_front(node->Right);
            }
        }


    }
    return answer;
}

int main() {
    Tree tree;
    int n = 0, number = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> number;
        tree.add(number);
    }

    //Заполняем очередь
    std::deque<int> answer = tree.pre_order();

    //Печатаем, пока очередь не опустеет
    while(!answer.empty()){
        std::cout << answer.front() << ' ';
        answer.pop_front();
    }

    std::cout << std::endl;

    return 0;
}
