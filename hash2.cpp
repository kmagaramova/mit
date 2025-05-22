#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

const int TABLE_SIZE = 30; // Размер хеш-таблицы (больше чем количество элементов)

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

// Основная хеш-функция (линейное хеширование)
int hashFunction(int experience) {
    return experience % TABLE_SIZE;
}

// Вспомогательная хеш-функция (метод умножения)
int auxiliaryHashFunction(int experience) {
    const double A = 0.6180339887; // Золотое сечение
    double val = experience * A;
    val -= (int)val;
    return (int)(TABLE_SIZE * val);
}

// Структура для хеш-таблицы
struct HashTable {
    date* table[TABLE_SIZE];
    bool occupied[TABLE_SIZE];

    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;
            occupied[i] = false;
        }
    }

    // Вставка элемента в хеш-таблицу
    void insert(date employee) {
        int index = hashFunction(employee.experience);
        int step = auxiliaryHashFunction(employee.experience);

        int i = 0;
        while (occupied[index] && i < TABLE_SIZE) {
            index = (index + step) % TABLE_SIZE;
            i++;
        }

        if (i < TABLE_SIZE) {
            table[index] = new date(employee);
            occupied[index] = true;
        }
        else {
            cout << "Хеш-таблица переполнена!" << endl;
        }
    }

    // Поиск элемента по стажу
    date* search(int experience) {
        int index = hashFunction(experience);
        int step = auxiliaryHashFunction(experience);

        int i = 0;
        while (occupied[index] && i < TABLE_SIZE) {
            if (table[index]->experience == experience) {
                return table[index];
            }
            index = (index + step) % TABLE_SIZE;
            i++;
        }

        return nullptr;
    }

    // Вывод хеш-таблицы с полной датой рождения
    void printTable() {
        cout << "Хеш-таблица:" << endl;
        cout << setw(5) << "Индекс" << setw(15) << "Фамилия" << setw(15) << "Должность"
            << setw(15) << "Дата рождения" << setw(10) << "Стаж" << setw(10) << "Зарплата" << endl;
        cout << "----------------------------------------------------------------------------" << endl;

        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (occupied[i]) {
                cout << setw(5) << i << setw(15) << table[i]->surname << setw(15) << table[i]->pos
                    << setw(10) << table[i]->birthDay << "." << setw(2) << table[i]->birthMonth << "." << setw(4) << table[i]->birthYear
                    << setw(10) << table[i]->experience << setw(10) << table[i]->salary << endl;
            }
            else {
                cout << setw(5) << i << setw(15) << "---" << setw(15) << "---"
                    << setw(15) << "---" << setw(10) << "---" << setw(10) << "---" << endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "");
    vector<date> employees = readFromFile("input.txt");

    if (employees.empty()) {
        cout << "Файл пуст или данные не были считаны." << endl;
        return 1;
    }

    // Создаем и заполняем хеш-таблицу
    HashTable hashTable;
    for (const auto& emp : employees) {
        hashTable.insert(emp);
    }

    // Выводим хеш-таблицу
    hashTable.printTable();

    // Демонстрация поиска
    int searchExp;
    cout << "\nВведите стаж для поиска: ";
    cin >> searchExp;

    date* found = hashTable.search(searchExp);
    if (found) {
        cout << "\nНайден сотрудник:" << endl;
        cout << "Фамилия: " << found->surname << endl;
        cout << "Должность: " << found->pos << endl;
        cout << "Дата рождения: " << found->birthDay << "." << found->birthMonth << "." << found->birthYear << endl;
        cout << "Стаж: " << found->experience << " лет" << endl;
        cout << "Зарплата: " << found->salary << " RUB" << endl;
    }
    else {
        cout << "Сотрудник со стажем " << searchExp << " лет не найден." << endl;
    }

    // Демонстрация добавления нового элемента
    date newEmployee;
    cout << "\nДобавление нового сотрудника:" << endl;
    cout << "Фамилия: "; cin >> newEmployee.surname;
    cout << "Должность: "; cin >> newEmployee.pos;
    cout << "День рождения: "; cin >> newEmployee.birthDay;
    cout << "Месяц рождения: "; cin >> newEmployee.birthMonth;
    cout << "Год рождения: "; cin >> newEmployee.birthYear;
    cout << "Стаж: "; cin >> newEmployee.experience;
    cout << "Зарплата: "; cin >> newEmployee.salary;

    hashTable.insert(newEmployee);
    cout << "\nНовый сотрудник добавлен в хеш-таблицу." << endl;
    hashTable.printTable();

    return 0;
}