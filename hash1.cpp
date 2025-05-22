#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

struct date {
    string surname;
    string pos;
    int birthDay;
    int birthMonth;
    int birthYear;
    int experience;
    int salary;
};

struct list {
    date inf;
    list* next;
    list* prev;
};

// Функция добавления элемента в конец списка
void push(list*& h, list*& t, date x) {
    list* r = new list;
    r->inf = x;
    r->next = NULL;
    if (!h && !t) {
        r->prev = NULL;
        h = r;
    }
    else {
        t->next = r;
        r->prev = t;
    }
    t = r;
}

// Функция печати списка
void print(list* h) {
    list* p = h;
    while (p) {
        cout << p->inf.surname << " " << p->inf.pos << " "
            << p->inf.birthDay << "." << p->inf.birthMonth << "." << p->inf.birthYear << " "
            << p->inf.experience << " years " << p->inf.salary << " RUB" << endl;
        p = p->next;
    }
}

// Функция для удаления элемента из двусвязного списка
void deleteNode(list*& head, list*& tail, list* nodeToDelete) {
    if (!nodeToDelete) return;

    // Обновляем связи соседних элементов
    if (nodeToDelete->prev) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    else {
        head = nodeToDelete->next; // Удаляем первый элемент
    }

    if (nodeToDelete->next) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    else {
        tail = nodeToDelete->prev; // Удаляем последний элемент
    }

    // Освобождаем память
    delete nodeToDelete;
}

// Хэш-функция (метод деления по зарплате)
int hashFunction(int salary, int tableSize) {
    return salary % tableSize;
}

// Функция вставки в хэш-таблицу
void insertToHashTable(list** hashTable, date employee, int tableSize) {
    int index = hashFunction(employee.salary, tableSize);
    if (!hashTable[index]) {
        list* head = NULL;
        list* tail = NULL;
        push(head, tail, employee);
        hashTable[index] = head;
    }
    else {
        list* tail = hashTable[index];
        while (tail->next) {
            tail = tail->next;
        }
        push(hashTable[index], tail, employee);
    }
}

// Функция вывода хэш-таблицы
void printHashTable(list** hashTable, int tableSize) {
    for (int i = 0; i < tableSize; i++) {
        cout << "Bucket " << i << ":" << endl;
        if (hashTable[i]) {
            print(hashTable[i]);
        }
        else {
            cout << "Empty" << endl;
        }
        cout << endl;
    }
}

// Функция поиска в хэш-таблице
void searchInHashTable(list** hashTable, int salary, int tableSize) {
    int index = hashFunction(salary, tableSize);
    cout << "Searching in bucket " << index << " for salary " << salary << ":" << endl;

    list* p = hashTable[index];
    bool found = false;
    while (p) {
        if (p->inf.salary == salary) {
            cout << "Found: " << p->inf.surname << " " << p->inf.pos << " "
                << p->inf.birthDay << "." << p->inf.birthMonth << "." << p->inf.birthYear << " "
                << p->inf.experience << " years " << p->inf.salary << " RUB" << endl;
            found = true;
        }
        p = p->next;
    }

    if (!found) {
        cout << "No employees with salary " << salary << " found." << endl;
    }
}

// Функция удаления из хэш-таблицы
void deleteFromHashTable(list** hashTable, int salary, int tableSize) {
    int index = hashFunction(salary, tableSize);
    cout << "Deleting from bucket " << index << " for salary " << salary << ":" << endl;

    list* p = hashTable[index];
    list* tail = nullptr;
    // Найдем хвост списка (если он есть)
    if (p) {
        tail = p;
        while (tail->next) {
            tail = tail->next;
        }
    }

    bool deleted = false;
    while (p) {
        if (p->inf.salary == salary) {
            list* nextNode = p->next; // Сохраняем указатель на следующий узел перед удалением
            cout << "Deleted: " << p->inf.surname << " " << p->inf.pos << " "
                << p->inf.birthDay << "." << p->inf.birthMonth << "." << p->inf.birthYear << " "
                << p->inf.experience << " years " << p->inf.salary << " RUB" << endl;
            deleteNode(hashTable[index], tail, p);
            deleted = true;
            p = nextNode; // Переходим к следующему узлу
        }
        else {
            p = p->next;
        }
    }

    if (!deleted) {
        cout << "No employees with salary " << salary << " found to delete." << endl;
    }
}
vector<date> readFromFile(const string& filename) {
    ifstream inFile(filename);
    vector<date> employees;
    date temp;

    while (inFile >> temp.surname >> temp.pos >> temp.birthDay >> temp.birthMonth >> temp.birthYear >> temp.experience >> temp.salary) {
        employees.push_back(temp);
    }
    inFile.close();
    return employees;
}

int main() {
    setlocale(LC_ALL, "");
    vector<date> employees = readFromFile("input.txt");

    if (employees.empty()) {
        cout << "Файл пуст или данные не были считаны." << endl;
        return 1;
    }

    // Определяем размер хэш-таблицы (не менее 20)
    int tableSize = employees.size() < 20 ? 20 : employees.size();

    // Инициализация хэш-таблицы
    list** hashTable = new list * [tableSize];
    for (int i = 0; i < tableSize; i++) {
        hashTable[i] = NULL;
    }

    // Заполнение хэш-таблицы
    for (const auto& emp : employees) {
        insertToHashTable(hashTable, emp, tableSize);
    }

    // Вывод хэш-таблицы
    cout << "Hash Table (size: " << tableSize << "):" << endl;
    printHashTable(hashTable, tableSize);
    cout << endl;

    // Демонстрация поиска
    int searchSalary;
    cout << "Enter salary to search: ";
    cin >> searchSalary;
    searchInHashTable(hashTable, searchSalary, tableSize);
    cout << endl;

    // Демонстрация удаления
    int deleteSalary;
    cout << "Enter salary to delete: ";
    cin >> deleteSalary;
    deleteFromHashTable(hashTable, deleteSalary, tableSize);
    cout << endl;

    // Вывод хэш-таблицы после удаления
    cout << "Hash Table after deletion:" << endl;
    printHashTable(hashTable, tableSize);
    cout << endl;

    // Демонстрация добавления нового элемента
    date newEmployee;
    cout << "Enter new employee data (surname position day month year experience salary): ";
    cin >> newEmployee.surname >> newEmployee.pos >> newEmployee.birthDay >> newEmployee.birthMonth
        >> newEmployee.birthYear >> newEmployee.experience >> newEmployee.salary;

    insertToHashTable(hashTable, newEmployee, tableSize);
    cout << endl;

    // Вывод хэш-таблицы после добавления
    cout << "Hash Table after insertion:" << endl;
    printHashTable(hashTable, tableSize);
    cout << endl;

    // Освобождение памяти
    for (int i = 0; i < tableSize; i++) {
        list* p = hashTable[i];
        while (p) {
            list* temp = p;
            p = p->next;
            delete temp;
        }
    }
    delete[] hashTable;

    return 0;
}