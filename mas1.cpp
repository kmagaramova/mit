//3) Столбцы по убыванию помощью поразрядной сортировки от младшего разряда к старшему (LSD).
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>

using namespace std;

ifstream in("inputmas.txt");
ofstream out("output.txt");

// Функция для получения цифры в определенном разряде
int ch(int num, int dg) {
    return (num / static_cast<int>(pow(10, dg))) % 10;// Возводит 10 в степень digit. 
    //Делит число на 10^digit, чтобы переместить нужный разряд в конец числа. 
}

// Поразрядная сортировка (LSD) для одного столбца
void Sort(vector<int>& column) {
    const int base = 10; // Основание системы счисления (10 для десятичной)
    int maxDigit = 0; // хранит максимальное количество разрядов в числах столбца.

    // Находим максимальное количество разрядов в столбце
    for (int num : column) {
        int digits = num == 0 ? 1 : static_cast<int>(log10(abs(num))) + 1; // Вычисляет логарифм числа по основанию 10. 
        //Это дает количество цифр в числе минус один. Если число равно 0, то количество цифр равно 1.
        if (digits > maxDigit) {
            maxDigit = digits;//обнова
        }
    }
    for (int digit = 0; digit < maxDigit; ++digit) { // Сортировка по каждому разряду
        vector<vector<int>> buckets(base);

        for (int num : column) { // Распределение чисел по корзинам
            int currentDigit = ch(num, digit); // Получаем цифру числа num в разряде digit.
            buckets[currentDigit].push_back(num); // Добавляем число в соответствующую корзину.
        }

        // Сбор чисел из корзин обратно в столбец (по убыванию)
        int index = 0;// запись чисел обратно в столбец после сортировки.
        for (int i = base - 1; i >= 0; --i) { // По убыванию
            for (int num : buckets[i]) {
                column[index++] = num;
            }
        }
    }
}

int main() {
    int n;
    in >> n;

    vector<vector<int>> ms(n, vector<int>(n));
    for (int i = 0; i < n; ++i) { // Считываем матрицу из файла
        for (int j = 0; j < n; ++j) {
            in >> ms[i][j];
        }
    }

    for (int j = 0; j < n; ++j) { // Сортируем каждый столбец по убыванию с помощью поразрядной сортировки
        vector<int> column(n);
        for (int i = 0; i < n; ++i) {
            column[i] = ms[i][j];
        }
        Sort(column);
        for (int i = 0; i < n; ++i) {
            ms[i][j] = column[i];
        }
    }

    for (int i = 0; i < n; ++i) { // Записываем отсортированную матрицу в файл
        for (int j = 0; j < n; ++j) {
            out << setw(5) << ms[i][j] << " ";
        }
        out << endl;
    }
    in.close();
    out.close();
    return 0;
}
