//Создать очередь из целых чисел. 
// Выполнить циклический сдвиг очереди так, чтобы на первом месте был первый четный элемент. 
// Например, было  1 3 5 2 5 1 4 5 3 4 5 2 1 . Стало 2 5 1 4 5 3 4 5 2 1 1 3 5 
#include <iostream>
using namespace std;

struct queue {
    int inf;
    queue* next;
};

void push(queue*& h, queue*& t, int x) {
    queue* r = new queue;
    r->inf = x;
    r->next = NULL;
    if (!h && !t) {
        h = t = r;
    }
    else {
        t->next = r;
        t = r;
    }
}

int pop(queue*& h, queue*& t) {
    queue* r = h;
    int i = h->inf;
    h = h->next;
    if (!h)
        t = NULL;
    delete r;
    return i;
}

bool isOdd(int n) {//если кратно двум
    return(n % 2 == 0);
}

void cyclicShift(queue*& h, queue*& t) {//функция циклического сдвига
    if (!h) return;

    // Ищем первое четное
    queue* current = h;
    while (current) {
        if (isOdd(current->inf)) {
            break;
        }
        // Перемещаем неподходящие элементы в конец
        int x = pop(h, t);
        push(h, t, x);
        current = h; // После перемещения начинаем снова с головы
    }
}

int main() {
    queue* h = NULL;
    queue* t = NULL;

    //ввод очереди
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        push(h, t, x);
    }

    cyclicShift(h, t);

    //вывод
    cout << "Result: ";
    while (h) {
        cout << pop(h, t) << " ";
    }
    cout << endl;

    return 0;
}