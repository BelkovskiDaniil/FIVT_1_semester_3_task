/*В город N приехал цирк с командой атлетов.
 * Они хотят удивить горожан города N — выстроить из своих тел башню максимальной высоты.
 * Башня — это цепочка атлетов, первый стоит на земле,
 * второй стоит у него на плечах, третий стоит на плечах у второго и т.д.
 * Каждый атлет характеризуется силой si (kg) и массой mi (kg). Сила — это максимальная масса,
 * которую атлет способен держать у себя на плечах. К сожалению ни один из атлетов не умеет программировать,
 * так как всю жизнь они занимались физической подготовкой, и у них не было времени на
 * изучение языков программирования. Помогите им, напишите программу, которая определит максимальную высоту башни,
 * которую они могут составить. Известно, что если атлет тяжелее, то он и сильнее: если mi>mj, то si > sj.
 * Атлеты равной массы могут иметь различную силу.*/

//https://contest.yandex.ru/contest/43508/run-report/80026068/

#include <iostream>
#include <vector>
#include <algorithm>

//Потоковый ввод
void entering(std::vector<int> &mass, std::vector<int> &power) {
    int massive = 0, power_near = 0;
    while(std::cin >> massive >> power_near) {
        mass.push_back(massive);
        power.push_back(power_near);
    }
}

//Сортируем атлетов по силе и массе
void sorting(std::vector<int> &mass, std::vector<int> &power) {
    sort(mass.begin(), mass.end());
    sort(power.begin(), power.end());
}

//Находим ответ
int finding_answer(std::vector<int> &mass, std::vector<int> &power) {
    int mass_local = mass[0], answer = 1;
    for(int i = 1; i < mass.size(); i++) {
        if(power[i] >= mass_local) {
            mass_local += mass[i];
            answer++;
        }
    }
    return answer;
}


int main() {
    std::vector<int> mass, power;
    entering(mass, power);
    sorting(mass, power);
    std::cout << finding_answer(mass, power);

    return 0;
}
