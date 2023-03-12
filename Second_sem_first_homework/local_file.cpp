/*Дано N кубиков. Требуется определить каким количеством способов
 *можно выстроить из этих кубиков пирамиду.
 *Каждый вышележащий слой пирамиды должен быть не больше нижележащего.
 *
 * https://contest.yandex.ru/contest/46386/run-report/83839374/
 */

#include <iostream>
#include <vector>

long long find_forms(int n, std::vector<std::vector<long long>>& answer_vector) {
    //Рассматриваем возможные формы, ходя по "столбцам" и "строкам"
    for (int i = 0; i < n + 1; i++) {
        for (int j = 2; j < n + 1; j++) {
            answer_vector[i][j] = answer_vector[i][j - 1];
            if (0 < i - j + 1){
                answer_vector[i][j] += answer_vector[i - j][j];
            }
        }
    }
    return answer_vector[n][n];
}

int main() {
    int n = 0;
    std::cin >> n;
    std::vector<std::vector<long long>> vector_local(n + 1, std::vector<long long>(n + 1));
    //Заполняем первые два столбца таблицы
    for (int i = 0; i < n + 1; i++) {
        vector_local[i][0] = 0;
        vector_local[i][1] = 1;
    }
    std::cout << find_forms(n, vector_local) << std::endl;
    return 0;
}