/*Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте
 * при добавлении элементов в случае, когда
 * коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления
 * строки из множества и проверки принадлежности данной строки множеству.
 * Вариант 1. Для разрешения коллизий используйте квадратичное пробирование.
 * i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.*/

//https://contest.yandex.ru/contest/43508/run-report/80025461/

#include <iostream>
#include <string>
#include <vector>

class String_Hash {
    const int a = 50;
public:
    int operator()(const std::string& key, int maxValue) const {
        int hash = 0;
        for(const char& c : key) {
            hash = (hash * a + c) % maxValue;
        }
        return hash;
    }
};

template<class T, class Hash = std::hash<T>, class Equality = std::equal_to<T>>

class HashTable {
public:
    bool Has(const T& key);
    bool Add(const T& key);
    bool Del(const T& key);
    HashTable(): size(0), power(8), loadFactor(0.75), table(power, nullptr), text(new Node("")) {}
    ~HashTable() {
        for (int i = 0; i < power; ++i) {
            if (table[i] != nullptr && table[i] != text)
                delete table[i] ;
        }
        delete text;
    }
    HashTable(const HashTable&) = delete;
    HashTable(const HashTable&&) = delete;
    HashTable& operator= (const HashTable&) = delete;
    HashTable& operator= (const HashTable&&) = delete;
private:
    struct Node {
        T key;
        Node(T key) : key(std::move(key)) {}
    };
    Node* text;
    int size;
    int power;
    const double loadFactor;
    std::vector<Node*> table;
    Hash hash;
    Equality equals;
    inline int quadratic_probing(const int hash, const int i) const;
    void Grow();
};

//Проверка на наличие
template<class T, class Hash, class Equality>
bool HashTable<T, Hash, Equality>::Has(const T& key) {
    int hash = this->hash(key, power);
    for (int i = 0; i < power; ++i) {
        hash = quadratic_probing(hash, i);
        if (table[hash] == nullptr) return false;
        if (table[hash] != text && equals(table[hash]->key, key)) return true;
    }
    return false;
}

//Добавление элемента
template<class T, class Hash, class Equality>
bool HashTable<T, Hash, Equality>::Add(const T& key) {
    int hash = this->hash(key, power);
    int placeToInsert = 0;
    for (int i = 0; i < power; ++i) {
        hash = quadratic_probing(hash, i);
        if (table[hash] == nullptr) {
            placeToInsert = hash;
            break;
        }
        if (table[hash] == text) {
            placeToInsert = hash;
        } else if (equals(table[hash]->key, key)) {
            return false;
        }
    }
    table[placeToInsert] =  new Node(key);
    ++size;
    if (size > power * loadFactor) Grow();
    return true;
}

//Увеличение таблицы
template<class T, class Hash, class Equality>
void HashTable<T, Hash, Equality>::Grow() {
    std::vector<Node*> oldTable = std::move(table);
    power *= 2;
    table = std::vector<Node*>(power, nullptr);
    for (Node* e : oldTable) {
        if (e != nullptr && e != text) {
            int hash = this->hash(e->key, power);
            for (int i = 0; table[hash] != nullptr && i < power; ++i) {
                hash = quadratic_probing(hash, i);
            }
            table[hash] = e;
        }
    }
}

//Удаление элемента
template<class T, class Hash, class Equality>
bool HashTable<T, Hash, Equality>::Del(const T& key) {
    int hash = this->hash(key, power);
    for (int i = 0; i < power; ++i) {
        hash = quadratic_probing(hash, i);
        if (table[hash] == nullptr) return false;
        if (table[hash] != text && equals(table[hash]->key, key)) {
            delete table[hash];
            table[hash] = text;
            --size;
            return true;
        }
    }
}

//Квадратичное пробирование
template<class T, class Hash, class Equality>
inline int HashTable<T, Hash, Equality>::quadratic_probing(int hashPrev, int i) const {
    return (hashPrev + i) % power;
}

int main() {
    HashTable<std::string, String_Hash> table;
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        if (command == '?') std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
        else if (command == '+') std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
        else if (command == '-') std::cout << (table.Del(value) ? "OK" : "FAIL") << std::endl;
    }
    return 0;
}