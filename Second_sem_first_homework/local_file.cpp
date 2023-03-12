/*Дано невзвешенное дерево. Расстоянием между двумя вершинами будем
 *называть количество ребер в пути, соединяющем эти две вершины.
 *Для каждой вершины определите сумму расстояний до всех остальных вершин.
 *Время работы должно быть O(n).
 *
 *https://contest.yandex.ru/contest/46386/run-report/83843069/
 */

#include <iostream>
#include <vector>


long long int find_distance(int i, std::vector<int>& counter, std::vector<std::vector<int>>& vector_local) {
    int distance = 0;
    for (int element : vector_local[i]) {
        distance += find_distance(element, counter, vector_local) + counter[element];
        counter[i] += counter[element];
    }
    counter[i] += 1;
    return distance;
}

int main() {
    int number, first, second;
    std::cin >> number;
    std::vector<std::vector<int>> vector_local(number);
    std::vector<int> prev(number);

    //Распределяем вершины и корни ребер
    for (int i = 0; i < number - 1; i++) {
        std::cin >> first;
        std::cin >> second;
        if (first > second)
            std::swap(first, second);
        vector_local[first].push_back(second);
        prev[second] = first;
    }

    std::vector<int> counter (number, 0);

    //Находим расстояние
    std::vector<long long int> dist(number);
    dist[0] = find_distance(0, counter, vector_local);
    std::cout << dist[0] << std::endl;

    for (int v = 1; v < number; ++v) {
        dist[v] = dist[prev[v]] - counter[v] + (number - counter[v]);
        std::cout << dist[v] << std::endl;
    }

    return 0;
}