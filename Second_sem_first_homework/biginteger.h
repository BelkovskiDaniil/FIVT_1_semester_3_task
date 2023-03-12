/*В этой задаче разрешается подключать <iostream>, <vector> и <string> и только их.

Напишите класс BigInteger для работы с длинными целыми числами. Должны поддерживаться операции:

сложение, вычитание, умножение, деление, остаток по модулю, работающие так же, как и для int;
составное присваивание с этими операциями;
унарный минус, префиксный и постфиксный инкремент и декремент,
операторы сравнения == != < > <= >=
вывод в поток и ввод из потока;
метод toString(), возвращающий строковое представление числа;
конструирование из int (в том числе неявное преобразование, когда это надо);
неявное преобразование в bool, когда это надо (должно работать в условных выражениях).
В вашем файле должна отсутствовать функция main(), а сам файл должен называться biginteger.h;
 в качестве компилятора необходимо указывать Make. Ваш код будет вставлен посредством команды #include<biginteger.h>
 в программу, содержащую тесты; вследствие этого, код необходимо отправлять в файле со строго соответствующим именем!

 https://contest.yandex.ru/contest/46478/run-report/83861744/
 */

#include <vector>
#include <string>
#include <iostream>

typedef std::vector<int> numbers_t;

class BigInteger {
    static const int BASE = 1000000000;

    void shift();

public:
    class divide_by_zero: public std::exception {  };

    std::vector<int> numbers;
    void del_zeroes();
    bool negative;

    BigInteger() : BigInteger(0) {};
    BigInteger(int n);
    BigInteger(const std::string &s);
    BigInteger(long long);
    explicit operator bool() const;
    BigInteger(numbers_t n, bool negative) : numbers(std::move(n)), negative(negative) {};

    operator std::string() const;
    BigInteger operator +() const;
    std::string toString() const;
    friend std::istream &operator >>(std::istream &istream, BigInteger &number);
    friend std::ostream &operator <<(std::ostream &ostream, const BigInteger &number);
    BigInteger &operator =(const BigInteger &value);
    friend BigInteger operator +(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator -(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator *(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator /(const BigInteger &left, const BigInteger &right);
    friend BigInteger operator %(const BigInteger &left, const BigInteger &right);
    BigInteger operator -() const;
    BigInteger &operator ++();
    BigInteger operator ++(int);
    BigInteger &operator --();
    BigInteger operator --(int);
    BigInteger &operator +=(const BigInteger &value);
    BigInteger &operator -=(const BigInteger &value);
    BigInteger &operator *=(const BigInteger &value);
    BigInteger &operator /=(const BigInteger &value);
    BigInteger &operator %=(const BigInteger &value);
    static BigInteger abs(const BigInteger &value);
    friend bool operator <(const BigInteger &left, const BigInteger &right);
    friend bool operator >(const BigInteger &left, const BigInteger &right);
    friend bool operator ==(const BigInteger &left, const BigInteger &right);
    friend bool operator <=(const BigInteger &left, const BigInteger &right);
    friend bool operator >=(const BigInteger &left, const BigInteger &right);
    friend bool operator !=(const BigInteger &left, const BigInteger &right);
};

BigInteger::BigInteger(int value) {
    numbers.clear();
    if (value < 0) {
        negative = true;
        value = -value;
    } else {
        negative = false;
    }

    do {
        numbers.push_back((int) (value % BASE));
        value /= BASE;
    } while (value);
}

BigInteger::BigInteger(const std::string &string) {
    if (string.empty()) {
        numbers.clear();
        numbers.emplace_back(0);
        negative = false;
        return;
    }
    numbers.clear();
    negative = string[0] == '-';
    for (int i = (int) string.length(); i > negative; i -= 9)
        if (i < 9)
            numbers.push_back(std::stoi(string.substr(negative, i - negative)));
        else
            numbers.push_back(std::stoi(string.substr(i - 9, 9)));
    del_zeroes();
}

void BigInteger::del_zeroes() {
    while (numbers.size() > 1 && numbers.back() == 0) {
        numbers.pop_back();
    }

    if (numbers.size() == 1 && numbers[0] == 0) negative = false;
}

BigInteger &BigInteger::operator=(const BigInteger &other) {
    numbers.resize(other.numbers.size());
    std::copy(other.numbers.begin(), other.numbers.end(), numbers.begin());
    negative = other.negative;
    return *this;
}

std::string BigInteger::toString() const {
    std::string result = negative ? "-" : "";

    result += std::to_string(numbers.empty() ? 0 : numbers.back());
    for (int i = (int)numbers.size() - 2; i >= 0; --i) {
        std::string tmp = std::to_string(numbers[i]);
        result += std::string(9 - tmp.size(), '0') + tmp;
    }
    return result;
}

std::ostream& operator <<(std::ostream& os, const BigInteger& bi) {
    os << bi.toString();
    return os;
}

std::istream& operator >> (std::istream& in, BigInteger& bi) {
    std::string str;
    in >> str;
    bi.numbers.clear();
    if (str.length() == 0) {
        bi.negative = false;
    }
    else {
        if (str[0] == '-') {
            str = str.substr(1);
            bi.negative = true;
        }
        else {
            bi.negative = false;
        }

        for (long long i = str.length(); i > 0; i -= 9) {
            if (i < 9)
                bi.numbers.push_back(atoi(str.substr(0, i).c_str()));
            else
                bi.numbers.push_back(atoi(str.substr(i - 9, 9).c_str()));
        }

        bi.del_zeroes();
    }
    return in;
}

bool operator ==(const BigInteger &left, const BigInteger &right) {
    if (left.negative != right.negative) return false;
    if (left.numbers.size() != right.numbers.size()) return false;
    for (size_t i = 0; i < left.numbers.size(); ++i) {
        if (left.numbers[i] != right.numbers[i]) return false;
    }
    return true;
}

BigInteger BigInteger::operator +() const {
    return BigInteger(*this);
}

BigInteger BigInteger::operator -() const {
    BigInteger copy(*this);
    if (this->numbers.empty() || (this->numbers.size() == 1 && this->numbers[0] == 0)) {
        copy.negative = false;
        return copy;
    }
    copy.negative = !copy.negative;
    return copy;
}

bool operator <(const BigInteger &left, const BigInteger &right) {
    if (left.negative != right.negative)
        return left.negative;

    if (left.numbers.size() != right.numbers.size())
        return (left.numbers.size() < right.numbers.size()) ^ left.negative;

    for (size_t i = left.numbers.size(); i > 0; --i)
        if (left.numbers[i - 1] != right.numbers[i - 1])
            return (left.numbers[i - 1] < right.numbers[i - 1]) ^ left.negative;

    return false;
}

bool operator !=(const BigInteger& left, const BigInteger& right) {
    return !(left == right);
}

bool operator <=(const BigInteger& left, const BigInteger& right) {
    return (left < right || left == right);
}

bool operator >(const BigInteger& left, const BigInteger& right) {
    return !(left <= right);
}

bool operator >=(const BigInteger& left, const BigInteger& right) {
    return !(left < right);
}

BigInteger operator +(const BigInteger &left, const BigInteger &right) {
    if (!left.negative && right.negative) {
        return left - -right;
    }

    if (left.negative && !right.negative) {
        return right - -left;
    }

    numbers_t resnumbers(left.numbers.size());
    std::copy(left.numbers.begin(), left.numbers.end(), resnumbers.begin());

    int carry = 0;
    for (size_t i = 0; i < std::max(left.numbers.size(), right.numbers.size()) || carry; ++i) {
        if (i == resnumbers.size()) resnumbers.push_back(0);

        resnumbers[i] += carry + (i < right.numbers.size() ? right.numbers[i] : 0);
        carry = resnumbers[i] >= BigInteger::BASE;
        if (carry) resnumbers[i] -= BigInteger::BASE;
    }

    BigInteger r(resnumbers, left.negative);
    return r;
}

BigInteger &BigInteger::operator +=(const BigInteger& value) {
    return *this = (*this + value);
}

BigInteger::BigInteger(long long i) {
    if (i < 0) negative = true;
    else negative = false;
    numbers.push_back(std::abs(i) % BigInteger::BASE);
    i /= BigInteger::BASE;
    if (i != 0) numbers.push_back(std::abs(i));
}

BigInteger &BigInteger::operator ++() {
    return (*this += 1);
}

BigInteger BigInteger::operator ++(int) {
    *this += 1;
    return *this - 1;
}

BigInteger &BigInteger::operator --() {
    return *this -= 1;
}

BigInteger BigInteger::operator --(int) {
    *this -= 1;
    return *this + 1;
}

BigInteger operator -(const BigInteger &left, const BigInteger &right) {
    if (!left.negative && right.negative) {
        return left + -right;
    }
    if (left.negative && !right.negative) {
        BigInteger tmp = -left + right;
        tmp.negative = true;
        return tmp;
    }

    if (BigInteger::abs(left) < BigInteger::abs(right)) {
        BigInteger result = (BigInteger::abs(right) - BigInteger::abs(left));
        result.negative = !left.negative;
        return result;
    }

    numbers_t resnumbers(left.numbers.size());
    std::copy(left.numbers.begin(), left.numbers.end(), resnumbers.begin());

    int carry = 0;
    for (size_t i = 0; i < right.numbers.size() || carry; ++i) {
        resnumbers[i] -= carry + (i < right.numbers.size() ? right.numbers[i] : 0);
        carry = resnumbers[i] < 0;
        if (carry) resnumbers[i] += BigInteger::BASE;
    }

    BigInteger result(resnumbers, left.negative);
    result.del_zeroes();
    return result;
}

BigInteger& BigInteger::operator -=(const BigInteger& value) {
    return *this = (*this - value);
}

BigInteger operator *(const BigInteger &left, const BigInteger &right) {
    numbers_t resnumbers(left.numbers.size() + right.numbers.size());
    for (size_t i = 0; i < left.numbers.size(); ++i)
        for (int j = 0, carry = 0; j < (int) right.numbers.size() || carry; ++j) {
            long long cur =
                    resnumbers[i + j] + left.numbers[i] * 1ll * (j < (int) right.numbers.size() ? right.numbers[j] : 0) + carry;
            resnumbers[i + j] = int(cur % BigInteger::BASE);
            carry = int(cur / BigInteger::BASE);
        }

    while (resnumbers.size() > 1 && resnumbers.back() == 0)
        resnumbers.pop_back();
    BigInteger result(resnumbers, !(resnumbers.size() == 1 && resnumbers[0] == 0) && (left.negative ^ right.negative));
    return result;
}

BigInteger& BigInteger::operator *=(const BigInteger& value) {
    return *this = (*this * value);
}

void BigInteger::shift() {
    if (numbers.size() == 0) {
        numbers.push_back(0);
        return;
    }
    numbers.push_back(numbers[numbers.size() - 1]);
    for (size_t i = numbers.size() - 2; i > 0; --i) numbers[i] = numbers[i - 1];
    numbers[0] = 0;
}

BigInteger BigInteger::abs(const BigInteger &value) {
    BigInteger result = value;
    result.negative = false;
    return result;
}

BigInteger operator /(const BigInteger &left, const BigInteger &right) {
    if (BigInteger::abs(left) < BigInteger::abs(right)) {
        return 0;
    }

    BigInteger abs_other = BigInteger::abs(right);
    BigInteger div_result, remainder;
    div_result.numbers.clear();
    remainder.numbers.clear();
    div_result.numbers.resize(left.numbers.size());
    for (long long i = (long long) (left.numbers.size()) - 1; i >= 0; --i) {
        remainder.shift();
        remainder.numbers[0] = left.numbers[i];
        remainder.del_zeroes();
        int x = 0, l = 0, r = BigInteger::BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger t = abs_other * m;
            if (t <= remainder) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }
        div_result.numbers[i] = x;
        remainder -= abs_other * x;

    }
    div_result.negative = left.negative ^ right.negative;
    div_result.del_zeroes();
    return div_result;
}

BigInteger& BigInteger::operator /=(const BigInteger& value) {
    return *this = (*this / value);
}

BigInteger operator %(const BigInteger& left, const BigInteger& right) {
    BigInteger result = left - (left / right) * right;
    return result;
}

BigInteger& BigInteger::operator %=(const BigInteger& value) {
    return *this = (*this % value);
}

BigInteger::operator bool() const {
    if (this->numbers.empty() || (this->numbers.size() == 1 && this->numbers[0] == 0)) {
        return false;
    }
    else {
        return true;
    }
}