#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;

// Структура для хранения данных о сотруднике
struct Employee {
    string surname;
    string position;
    int birthDay;
    int birthMonth;
    int birthYear;
    int experience;
    int salary;
    bool isDeleted;
};

struct Node {
    Employee data; 
    Node* prev; 
    Node* next; 
};

// Класс двусвязного списка
class DoublyLinkedList {
private:
    Node* head; 
    Node* tail;

public:
    // Конструктор
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    // Метод для вставки элемента в список
    void insert(const Employee& emp) {
        Node* newNode = new Node{ emp, nullptr, nullptr };
        if (!head) {
            // Если список пуст, новый элемент становится и головой и хвостом
            head = tail = newNode;
        }
        else {
            // Добавляем элемент в конец списка
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // Метод для поиска элемента по стажу
    Node* searchByExperience(int exp) {
        Node* current = head;
        while (current) {
            if (current->data.experience == exp && !current->data.isDeleted) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Метод для вывода содержимого списка
    void display() {
        Node* current = head;
        while (current) {
            if (!current->data.isDeleted) {
                cout << current->data.surname << " "
                    << current->data.position << " "
                    << current->data.birthDay << "."
                    << current->data.birthMonth << "."
                    << current->data.birthYear << " "
                    << current->data.experience << " years "
                    << current->data.salary << " USD" << endl;
            }
            current = current->next;
        }
    }

    // Деструктор для освобождения памяти
    ~DoublyLinkedList() {
        Node* current = head;
        while (current) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }
};

// Класс хеш-таблицы с закрытым хешированием
class HashTable {
private:
    vector<Employee> table;     // Вектор для хранения элементов
    int size;                   // Размер таблицы
    vector<bool> isOccupied;    // Вектор для отметки занятых ячеек

    // Основная хеш-функция (линейное хеширование по фамилии)
    int primaryHash(const string& surname) {
        int hash = 0;
        for (char c : surname) {
            hash = (hash * 31 + c) % size;  // Простое хеширование строки
        }
        return hash;
    }

    // Вспомогательная хеш-функция (метод умножения по стажу)
    int secondaryHash(int experience) {
        const double A = 0.6180339887;      // Константа (золотое сечение)
        double val = experience * A;
        val -= (int)val;                    // Дробная часть
        return (int)(size * val);           // Масштабирование до размера таблицы
    }

public:
    // Конструктор с заданным размером таблицы
    HashTable(int tableSize) : size(tableSize) {
        table.resize(size);
        isOccupied.resize(size, false);
    }

    // Метод для вставки элемента в таблицу
    bool insert(const Employee& emp) {
        int hash = primaryHash(emp.surname);        // Вычисляем основную хеш
        int step = secondaryHash(emp.experience);   // Вычисляем шаг для линейного пробирования

        for (int i = 0; i < size; i++) {
            int index = (hash + i * step) % size;   // Линейное пробирование

            // Если ячейка свободна или содержит удаленный элемент
            if (!isOccupied[index] || table[index].isDeleted) {
                table[index] = emp;
                table[index].isDeleted = false;
                isOccupied[index] = true;
                return true;
            }
        }
        return false;  // Таблица переполнена
    }

    // Метод для поиска элемента по фамилии и стажу
    Employee* search(const string& surname, int experience) {
        int hash = primaryHash(surname);
        int step = secondaryHash(experience);

        for (int i = 0; i < size; i++) {
            int index = (hash + i * step) % size;

            // Если ячейка пуста, элемент не найден
            if (!isOccupied[index]) {
                return nullptr;
            }

            // Проверяем, не удален ли элемент и совпадают ли данные
            if (!table[index].isDeleted &&
                table[index].surname == surname &&
                table[index].experience == experience) {
                return &table[index];
            }
        }
        return nullptr;
    }

    // Метод для удаления элемента
    bool remove(const string& surname, int experience) {
        Employee* emp = search(surname, experience);
        if (emp) {
            emp->isDeleted = true;  // Помечаем как удаленный
            return true;
        }
        return false;
    }

    // Метод для вывода содержимого таблицы
    void displayTable() {
        cout << "Hash Table Contents:" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(15) << "Index" << setw(20) << "Surname" << setw(15) << "Position"
            << setw(12) << "Birth Date" << setw(10) << "Experience" << setw(10) << "Salary" << endl;
        cout << "------------------------------------------------------------" << endl;

        for (int i = 0; i < size; i++) {
            if (isOccupied[i] && !table[i].isDeleted) {
                cout << left << setw(15) << i
                    << setw(20) << table[i].surname
                    << setw(15) << table[i].position
                    << setw(2) << table[i].birthDay << "."
                    << setw(2) << table[i].birthMonth << "."
                    << setw(6) << table[i].birthYear
                    << setw(10) << table[i].experience
                    << setw(10) << table[i].salary << endl;
            }
            else if (isOccupied[i]) {
                cout << left << setw(15) << i << "DELETED" << endl;
            }
        }
        cout << "------------------------------------------------------------" << endl;
    }
};

// Функция для чтения данных из файла
vector<Employee> readFromFile(const string& filename) {
    ifstream inFile(filename);
    vector<Employee> employees;
    Employee temp;

    while (inFile >> temp.surname >> temp.position >> temp.birthDay
        >> temp.birthMonth >> temp.birthYear >> temp.experience >> temp.salary) {
        temp.isDeleted = false;
        employees.push_back(temp);
    }
    inFile.close();
    return employees;
}

int main() {
    setlocale(LC_ALL, "RUS");  // Для поддержки русского языка

    // Чтение данных из файла
    vector<Employee> employees = readFromFile("input.txt");

    if (employees.empty()) {
        cout << "Файл пуст или данные не были считаны." << endl;
        return 1;
    }

    // Создаем хеш-таблицу с размером в 2 раза больше количества элементов
    HashTable hashTable(employees.size() * 2);

    // Заполняем хеш-таблицу
    for (const auto& emp : employees) {
        if (!hashTable.insert(emp)) {
            cout << "Не удалось вставить элемент " << emp.surname << endl;
        }
    }

    // Выводим хеш-таблицу
    hashTable.displayTable();

    // Демонстрация поиска
    string searchSurname;
    int searchExp;
    cout << "\nВведите фамилию и стаж для поиска: ";
    cin >> searchSurname >> searchExp;

    Employee* found = hashTable.search(searchSurname, searchExp);
    if (found) {
        cout << "\nНайден сотрудник:" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << "Фамилия" << setw(15) << "Должность"
            << setw(12) << "Дата рождения" << setw(10) << "Стаж" << setw(10) << "Зарплата" << endl;
        cout << "------------------------------------------------------------" << endl;
        cout << left << setw(20) << found->surname
            << setw(15) << found->position
            << setw(2) << found->birthDay << "."
            << setw(2) << found->birthMonth << "."
            << setw(6) << found->birthYear
            << setw(10) << found->experience
            << setw(10) << found->salary << endl;
        cout << "------------------------------------------------------------" << endl;
    }
    else {
        cout << "Сотрудник не найден." << endl;
    }

    // Демонстрация добавления нового элемента
    Employee newEmp;
    cout << "\nВведите данные нового сотрудника:" << endl;
    cout << "Фамилия: "; cin >> newEmp.surname;
    cout << "Должность: "; cin >> newEmp.position;
    cout << "День рождения: "; cin >> newEmp.birthDay;
    cout << "Месяц рождения: "; cin >> newEmp.birthMonth;
    cout << "Год рождения: "; cin >> newEmp.birthYear;
    cout << "Стаж работы: "; cin >> newEmp.experience;
    cout << "Зарплата: "; cin >> newEmp.salary;
    newEmp.isDeleted = false;

    if (hashTable.insert(newEmp)) {
        cout << "\nНовый сотрудник добавлен. Обновленная хеш-таблица:" << endl;
        hashTable.displayTable();
    }
    else {
        cout << "Не удалось добавить нового сотрудника (таблица переполнена)." << endl;
    }

    // Демонстрация удаления
    string delSurname;
    int delExp;
    cout << "\nВведите фамилию и стаж сотрудника для удаления: ";
    cin >> delSurname >> delExp;

    if (hashTable.remove(delSurname, delExp)) {
        cout << "Сотрудник удален. Обновленная хеш-таблица:" << endl;
        hashTable.displayTable();
    }
    else {
        cout << "Сотрудник не найден." << endl;
    }

    return 0;
}