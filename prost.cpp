#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Функция для возведения в степень
long long powerMod(long long a, long long b, long long n) {
    long long result = 1;
    a = a % n;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result * a) % n;
        b = b >> 1;
        a = (a * a) % n;
    }
    return result;
}

// Проверка Миллера-Рабина для одного свидетеля
bool millerTest(long long d, long long n) {
    long long a = 2 + rand() % (n - 4); // Случайное число от 2 до n-2
    long long x = powerMod(a, d, n);
    if (x == 1 || x == n - 1)
        return true;
    while (d != n - 1) {
        x = (x * x) % n;
        d *= 2;
        if (x == 1) return false;
        if (x == n - 1) return true;
    }
    return false;
}

// Основная функция проверки на простоту (k — количество раундов)
bool isPrime(long long n, int k = 5) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;
    // Находим d такое, что n-1 = 2^s * d
    long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;
    // Проверяем k раз
    for (int i = 0; i < k; i++) {
        if (!millerTest(d, n))
            return false;
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "RUS");
    srand(time(0)); // Инициализация генератора случайных чисел
    long long num;
    cout << "Введите число для проверки на простоту: ";
    cin >> num;
    if (isPrime(num))
        cout << num << " — простое число" << endl;
    else
        cout << num << " — составное число" << endl;
    return 0;
}