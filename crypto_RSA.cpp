#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/multiprecision/cpp_int.hpp>

// Использование типа uint1024_t из библиотеки Boost для работы с большими числами
typedef boost::multiprecision::int1024_t lngen;

// Бинарное возведение в степень
lngen bin_pow(lngen base, lngen exp, lngen mod) {
    lngen result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return result;
}

// Вероятностный тест Миллера-Рабина для проверки на псевдопростоту
bool Miller_Rabin_test(lngen n, int k = 512) {
    // Базовые проверки
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0) return false;

    // Разложение n-1 = 2^s * d
    lngen d = n - 1;
    int s = 0;
    while (d % 2 == 0) {
        d >>= 1;
        s++;
    }

    // Инициализация генератора случайных чисел
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<lngen> dis(2, n - 2);

    // Выполнение k тестов
    for (int i = 0; i < k; i++) {
        lngen a = dis(gen);
        lngen x = bin_pow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        for (int j = 0; j < s - 1; j++) {
            x = bin_pow(x, 2, n);
            if (x == n - 1)
                break;
        }
        if (x != n - 1)
            return false;
    }
    return true;
}

// Генерация псевдопростых чисел в заданном промежутке
lngen random_prime(const lngen& begin, const lngen& end) {
    // Проверка корректности диапазона
    if (begin > end) {
        std::cerr << "Ошибка: begin должен быть меньше или равен end." << std::endl;
        exit(1);
    }

    // Инициализация генератора случайных чисел
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<lngen> probably_prime(begin, end);
    lngen p;

    while (true) {
        p = probably_prime(gen);
        if (Miller_Rabin_test(p)) {
            return p;
        }
    }
}

// Расширенный алгоритм Евклида
lngen exgcd(lngen a, lngen b, lngen &x, lngen &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    lngen x1, y1;
    lngen gcd = exgcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;
    return gcd;
}

int main() {
    lngen x = 12345, p, q, e, d = 1, t = 1, n, phi, ciphertext;
    const lngen start = static_cast<lngen>(2) << 64, finish = static_cast<lngen>(2) << 66;

    // Генерация двух больших псевдопростых чисел
    p = random_prime(start, finish);
    q = random_prime(start, finish);

    // Вычисление значения функции Эйлера для n
    phi = (p - 1) * (q - 1);
    n = p * q;

    // Выбор открытого ключа e
    e = random_prime(phi, static_cast<lngen>(2) << 64);

    std::cout << "p = " << p << " q = " << q << " phi = " << phi << " n = " << n << " e = " << e << '\n';

    // Нахождение обратного элемента d для e по модулю phi
    exgcd(e, phi, d, t);
    d = (d + phi) % phi;

    std::cout << "d = " << d << '\n';

    // Шифрование сообщения x
    ciphertext = bin_pow(x, e, n);
    std::cout << "ciphertext is " << ciphertext << '\n';

    // Дешифрование зашифрованного сообщения
    std::cout << bin_pow(ciphertext, d, n) << '\n';

    // Проверка корректности ключей
    std::cout << (d * e) % phi;

    std::cout << "\n \n \n";
    return 0;
}

