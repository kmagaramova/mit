//Создать стек, содержащий целые числа. После  всех минимальных чисел вставить максимальное число. 
// Порядок следования в результате должен совпадать с порядком следования ввода. 
// Например, было 2 8 4 1 2 8 8 1 2 8 Стало 2 8 4 1 8 2 8  8 1 8 2 8
#include<iostream>
using namespace std;

struct stack {
    int inf;
    stack* next;
};

void push(stack*& h, int x) {// вставка элемента в стек
    stack* r = new stack;
    r->inf = x;
    r->next = h;
    h = r;
}

int pop(stack*& h) {// удаление элемента из стека
    int i = h->inf;
    stack* r = h;
    h = h->next;
    delete r;
    return i;
}

void reverse(stack*& h) {// обращение стека
    stack* head1 = NULL;
    while (h)
        push(head1, pop(h));
    h = head1;
}

void findMinMax(stack* h, int& min_val, int& max_val) {//поиск максимума и минимума
    if (!h) return;

    min_val = h->inf;
    max_val = h->inf;

    while (h) {
        if (h->inf < min_val) min_val = h->inf;
        if (h->inf > max_val) max_val = h->inf;
        h = h->next;
    }
}

stack* createResultStack(stack*& h, int min_val, int max_val) {//создание результирующего стека
    stack* new_head = NULL;
    while (h) {
        int val = pop(h);
        push(new_head, val);
        if (val == min_val) {
            push(new_head, max_val);
        }
    }
    reverse(new_head);
    return new_head;
}

int main() {
    stack* head = NULL;
    int n, x, min_val, max_val;

    //ввод стека
    cout << "Enter the number of elements: ";
    cin >> n;
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> x;
        push(head, x);
    }

    findMinMax(head, min_val, max_val);

    reverse(head);
    stack* result = createResultStack(head, min_val, max_val);

    //вывод стека
    cout << "Result: ";
    stack* temp = result;
    while (temp) {
        cout << temp->inf << " ";
        temp = temp->next;
    }
    cout << endl;

    return 0;
}