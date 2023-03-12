/*В дощечке в один ряд вбиты гвоздики.
 * Любые два гвоздика можно соединить ниточкой.
 * Требуется соединить некоторые пары гвоздиков ниточками так, чтобы к каждому
 * гвоздику была привязана хотя бы одна ниточка, а суммарная длина всех ниточек была минимальна.
 *
 * https://contest.yandex.ru/contest/46386/run-report/83838568/
 */

#include <iostream>
#include "algorithm"

int main() {
    int n, number;
    std::cin >> n;
    int *array = new int[n];
    int *answer_array = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> number;
        array[i] = number;
    }
    for (int i = 0; i < n; i++) {
        answer_array[i] = 0;
    }
    std::vector<int> vector (array, array + n);
    sort (vector.begin(), vector.end());
    answer_array[1] = vector[1] - vector[0];
    if (n > 2) {
        answer_array[2] = vector[2] - vector[0];
        for (int i = 3; i < n; i++) {
            answer_array[i] = std::min(answer_array[i - 2], answer_array[i - 1]) + vector[i] - vector[i - 1];
        }
    }
    std::cout << answer_array[n - 1];
}