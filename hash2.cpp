#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

const int TABLE_SIZE = 30;

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

// Функция добавления элемента в конец двусвязного списка
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


void print(list* h) {
    list* p = h;
    while (p) {

        cout << p->inf.surname << " " << p->inf.pos << " "
            << p->inf.birthDay << "." << p->inf.birthMonth << "." << p->inf.birthYear << " "
            << p->inf.experience << " years " << p->inf.salary << " RUB" << endl;
        p = p->next;
    }
}


void deleteNode(list*& head, list*& tail, list* nodeToDelete) {
    if (!nodeToDelete) return;


    if (nodeToDelete->prev) {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    else {
        head = nodeToDelete->next;
    }

    if (nodeToDelete->next) {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    else {
        tail = nodeToDelete->prev;
    }

    delete nodeToDelete;
}

// Функция чтения данных о сотрудниках из файла
vector<date> readFromFile(const string& filename) {
    ifstream inFile(filename);
    vector<date> employees;
    date temp;


    while (inFile >> temp.surname >> temp.pos >> temp.birthDay
        >> temp.birthMonth >> temp.birthYear >> temp.experience >> temp.salary) {
        employees.push_back(temp);
    }
    inFile.close();
    return employees;
}

// Основная хеш-функция (метод деления)
int hashFunction(int experience) {
    return experience % TABLE_SIZE;
}

// Вспомогательная хеш-функция (метод умножения)
int auxiliaryHashFunction(int experience) {
    const double A = 0.6180339887;
    double val = experience * A;
    val -= (int)val;
    return (int)(TABLE_SIZE * val);
}

// Класс хеш-таблицы с закрытым хешированием
struct HashTable {
    date* table[TABLE_SIZE];     // Массив указателей на данные
    bool occupied[TABLE_SIZE];   // Массив флагов занятости

    // Конструктор - инициализация таблицы
    HashTable() {
        for (int i = 0; i < TABLE_SIZE; ++i) {
            table[i] = nullptr;      // Все ячейки пусты
            occupied[i] = false;     // Все ячейки свободны
        }
    }

    // Метод вставки элемента в хеш-таблицу
    void insert(date employee) {
        int index = hashFunction(employee.experience);       // Первичный индекс
        int step = auxiliaryHashFunction(employee.experience); // Шаг для разрешения коллизий

        int i = 0;
        // Ищем свободную ячейку
        while (occupied[index] && i < TABLE_SIZE) {
            index = (index + step) % TABLE_SIZE; // Линейное зондирование
            i++;
        }

        // Если нашли свободную ячейку
        if (i < TABLE_SIZE) {
            table[index] = new date(employee); // Выделяем память и копируем данные
            occupied[index] = true;            // Помечаем ячейку как занятую
        }
        else {
            cout << "Хеш-таблица переполнена!" << endl;
        }
    }

    // Метод поиска всех сотрудников с заданным стажем
    vector<date*> search(int experience) {
        vector<date*> results;    // Вектор для хранения результатов

        // Проходим по всей таблице
        for (int i = 0; i < TABLE_SIZE; ++i) {
            // Если ячейка занята и стаж совпадает
            if (occupied[i] && table[i]->experience == experience) {
                results.push_back(table[i]); // Добавляем в результаты
            }
        }

        return results; // Возвращаем найденные элементы
    }

    // Метод вывода содержимого хеш-таблицы
    void printTable() {
        cout << "Хеш-таблица:" << endl;
        // Заголовок таблицы
        cout << setw(5) << "Индекс" << setw(15) << "Фамилия" << setw(15) << "Должность"
            << setw(15) << "Дата рождения" << setw(10) << "Стаж" << setw(10) << "Зарплата" << endl;
        cout << "----------------------------------------------------------------------------" << endl;

        // Вывод содержимого каждой ячейки
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (occupied[i]) {
                // Вывод данных о сотруднике
                cout << setw(5) << i << setw(15) << table[i]->surname << setw(15) << table[i]->pos
                    << setw(10) << table[i]->birthDay << "." << setw(2) << table[i]->birthMonth
                    << "." << setw(4) << table[i]->birthYear
                    << setw(10) << table[i]->experience << setw(10) << table[i]->salary << endl;
            }
            else {
                // Вывод пустой ячейки
                cout << setw(5) << i << setw(15) << "---" << setw(15) << "---"
                    << setw(15) << "---" << setw(10) << "---" << setw(10) << "---" << endl;
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, ""); // Для корректного вывода русских символов

    // Чтение данных из файла
    vector<date> employees = readFromFile("input.txt");
    if (employees.empty()) {
        cout << "Файл пуст или данные не были считаны." << endl;
        return 1;
    }

    // Создание и заполнение хеш-таблицы
    HashTable hashTable;
    for (const auto& emp : employees) {
        hashTable.insert(emp);
    }

    // Вывод содержимого хеш-таблицы
    hashTable.printTable();

    // Поиск сотрудников по стажу
    int searchExp;
    cout << "\nВведите стаж для поиска: ";
    cin >> searchExp;

    vector<date*> found = hashTable.search(searchExp);
    if (!found.empty()) {
        cout << "\nНайдены сотрудники со стажем " << searchExp << " лет:" << endl;
        cout << "----------------------------------------------------------------------------" << endl;
        cout << setw(15) << "Фамилия" << setw(15) << "Должность"
            << setw(15) << "Дата рождения" << setw(10) << "Стаж" << setw(10) << "Зарплата" << endl;
        cout << "----------------------------------------------------------------------------" << endl;

        // Вывод найденных сотрудников
        for (auto emp : found) {
            cout << setw(15) << emp->surname << setw(15) << emp->pos
                << setw(10) << emp->birthDay << "." << setw(2) << emp->birthMonth
                << "." << setw(4) << emp->birthYear
                << setw(10) << emp->experience << setw(10) << emp->salary << endl;
        }
    }
    else {
        cout << "Сотрудники со стажем " << searchExp << " лет не найдены." << endl;
    }

    // Добавление нового сотрудника
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