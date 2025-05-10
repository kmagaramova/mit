//Создать очередь, содержащую целые числа. 
// После всех чисел, равных минимальному числу, вставить последний четный элемент. 
// Например, было  2 8 2 1 6 8 8 1 2 2 8 2 1. Стало  2 8 2 1 2 6 8 8 1 2 2 2 8 2 1 2 
#include <iostream>
using namespace std;

struct queue {
    int inf;
    queue* next;
};

void push(queue*& h, queue*& t, int x) {// вставка элемента в очередь
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

int pop(queue*& h, queue*& t) {//удаление элемента из очереди
    if (!h) return -1;

    queue* r = h;
    int i = h->inf;
    h = h->next;
    if (!h)
        t = NULL;
    delete r;
    return i;
}

void processQueue(queue*& h, queue*& t) {
    if (!h) return;

    // 1. Находим минимальный элемент и последний четный
    int minVal = h->inf;
    int lastEven = -1;

    // Первый проход: находим minVal и lastEven
    queue* current = h;
    while (current) {
        if (current->inf < minVal) {
            minVal = current->inf;
        }
        if (current->inf % 2 == 0) {
            lastEven = current->inf;
        }
        current = current->next;
    }

    // Если нет четных элементов, ничего не делаем
    if (lastEven == -1) return;

    // 2. Создаем новую очередь с вставками
    queue* newH = NULL;
    queue* newT = NULL;

    // Второй проход: вставляем lastEven после каждого minVal
    while (h) {
        int x = pop(h, t);
        push(newH, newT, x);

        if (x == minVal){
            push(newH, newT, lastEven);
        }
    }

    // Переносим обратно в исходную очередь
    h = newH;
    t = newT;
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

    processQueue(h, t);

    // Выводим результат
    cout << "Result: ";
    while (h) {
        cout << pop(h, t) << " ";
    }
    cout << endl;

    return 0;
}