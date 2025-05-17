#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdexcept>

using namespace std;

// Функция для сортировки вставками с оптимизацией
void insertion_sort(vector<int>& arr) {
    for (size_t i = 1; i < arr.size(); ++i) {
        int key = arr[i];
        size_t j = i;

        // Сдвигаем элементы большие чем key вправо
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

void process_matrix(const string& input_file, const string& output_file) {
    ifstream in(input_file);
    ofstream out(output_file);

    if (!in.is_open()) {
        throw runtime_error("Cannot open input file: " + input_file);
    }
    if (!out.is_open()) {
        throw runtime_error("Cannot open output file: " + output_file);
    }

    int n;
    in >> n;
    if (n <= 0) {
        throw invalid_argument("Matrix size must be positive");
    }

    vector<vector<int>> matrix(n, vector<int>(n));

    // Чтение матрицы
    for (auto& row : matrix) {
        for (auto& elem : row) {
            if (!(in >> elem)) {
                throw runtime_error("Error reading matrix elements");
            }
        }
    }

    // Обработка диагоналей
    for (int k = 0; k < 2 * n - 1; ++k) {
        vector<int> diagonal;

        // Собираем диагональ
        for (int i = max(0, k - n + 1); i <= min(k, n - 1); ++i) {
            int j = k - i;
            diagonal.push_back(matrix[i][j]);
        }

        // Сортировка
        insertion_sort(diagonal);

        // Запись обратно
        size_t idx = 0;
        for (int i = max(0, k - n + 1); i <= min(k, n - 1); ++i) {
            int j = k - i;
            matrix[i][j] = diagonal[idx++];
        }
    }

    // Вывод результата
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            out << setw(5) << elem;
        }
        out << '\n';
    }
}

int main() {
    try {
        process_matrix("inputmas.txt", "output.txt");
        cout << "Matrix processed successfully\n";
        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}