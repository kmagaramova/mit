//Создать стек, содержащий целые числа от 0 до 20. В новый стек вывести сначала однозначные, потом двузначные числа. 
// Порядок следования чисел в новом стеке должен совпадать с первоначальным. 
// Например, было 1 20 13 4 15 16 7 18 9, стало 1 4 7 9 20 13 15 16 18
#include<iostream>
using namespace std;

struct stack {
    int inf;
    stack* next;
};

void push(stack*& h, int x) { // вставка элемента в стек
    stack* r = new stack;
    r->inf = x;
    r->next = h;
    h = r;
}

int pop(stack*& h) { // удаление элемента из стека
    int i = h->inf;
    stack* r = h;
    h = h->next;
    delete r;
    return i;
}

void reverse(stack*& h) { // обращение стека
    stack* head1 = NULL;
    while (h)
        push(head1, pop(h));
    h = head1;
}


stack* separateNumbers(stack*& h) {
    stack* singleDigit = NULL; // для однозначных чисел (0-9)
    stack* doubleDigit = NULL; // для двузначных чисел (10-20)
    stack* temp = NULL;        // временный стек для перестановки

    // Переворачиваем стек, чтобы обрабатывать числа в исходном порядке
    reverse(h);

    // Распределяем числа по стекам
    while (h) {
        int x = pop(h);
        if (x >= 0 && x < 10) {
            push(singleDigit, x);
        }
        else {
            push(doubleDigit, x);
        }
    }

    // Переворачиваем стеки, чтобы восстановить исходный порядок
    reverse(doubleDigit);

    // Собираем результат: сначала однозначные, затем двузначные
    stack* res = NULL;

    // Добавляем двузначные числа во временный стек
    while (doubleDigit) {
        push(temp, pop(doubleDigit));
    }

    // Добавляем однозначные числа в результат
    while (singleDigit) {
        push(res, pop(singleDigit));
    }

    // Добавляем двузначные числа в результат
    while (temp) {
        push(res, pop(temp));
    }

    // Переворачиваем результат, чтобы получить правильный порядок
    reverse(res);

    return res;
}

int main() {
    setlocale(LC_ALL, "Russian");
    stack* h = NULL;
    int n, x;

    cout << "n = ";
    cin >> n;
    //Ввод с проверкой корректности ввода по условию
    for (int i = 0; i < n; i++) {
        cin >> x;
        while (x < 0 || x > 20) { // Проверка на корректность ввода
            cout << "Ошибка! Число должно быть от 0 до 20. Повторите ввод: ";
            cin >> x;
        }
        push(h, x);
    }

   reverse(h); // Переворачиваем для вывода в исходном порядке

    stack* res = separateNumbers(h);

    cout << "Результат: ";
    while (res) {
        cout << pop(res) << " ";
    }
    cout << endl;

    return 0;
}