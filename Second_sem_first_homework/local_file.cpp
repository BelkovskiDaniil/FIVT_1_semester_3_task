/*Даны два "длинных" неотрицательных числа. Требуется найти результат их сложения.
 *
 *https://contest.yandex.ru/contest/46386/run-report/83845057/
 */

#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>

int main() {
    std::string biggest, smallest, str, first_number, second_number, answer, answer_new;
    int mem = 0, number, sum, end;
    std::cin >> first_number;
    std::cin >> second_number;

    //Находим наибольшее число
    if (first_number.size() >= second_number.size()) {
        biggest = first_number;
        smallest = second_number;
    }
    else {
        biggest = second_number;
        smallest = first_number;
    }

    //Дописываем нули меньшему числу и складываем столбиком
    number = abs(biggest.size() - smallest.size());
    for (int i = 0; i < number; i++) smallest = '0' + smallest;
    for (int i = biggest.size() - 1; i >= 0; i--) {
        sum = (biggest[i] - '0') + (smallest[i] - '0') + mem;
        end = sum % 10;
        str += end + '0';
        if (i == 0) str += std::to_string(sum);
        mem = sum / 10;
    }
    reverse(str.begin(), str.end());
    answer = str.erase(0, 1);
    if (answer.length() >= 2 && answer[0] == '-' && answer[1] == '-') answer_new = answer.substr(1, answer.length() - 1);
    else answer_new = answer;
    std::cout << answer_new << std::endl;
    return 0;
}