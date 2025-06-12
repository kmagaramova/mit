#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Функция для сложения двух матриц
vector<vector<int>> m_plus(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

// Функция для вычитания двух матриц
vector<vector<int>> m_minus(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();
    vector<vector<int>> C(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

// Функция для разделения матрицы на 4 подматрицы
void split_matrix(const vector<vector<int>>& original,
    vector<vector<int>>& part,
    int row_start, int row_end,
    int col_start, int col_end) {
    for (int i1 = 0, i2 = row_start; i2 < row_end; i1++, i2++) {
        for (int j1 = 0, j2 = col_start; j2 < col_end; j1++, j2++) {
            part[i1][j1] = original[i2][j2];
        }
    }
}

// Функция для объединения 4 подматриц в одну
void join_matrix(vector<vector<int>>& result,
    const vector<vector<int>>& part,
    int row_start, int row_end,
    int col_start, int col_end) {
    for (int i1 = 0, i2 = row_start; i2 < row_end; i1++, i2++) {
        for (int j1 = 0, j2 = col_start; j2 < col_end; j1++, j2++) {
            result[i2][j2] = part[i1][j1];
        }
    }
}

// Основная функция умножения матриц по Штрассену
vector<vector<int>> strassen_multiply(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();

    // Базовый случай: если матрица маленькая, используем стандартное умножение
    if (n <= 64) {
        vector<vector<int>> C(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                for (int j = 0; j < n; j++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }
        return C;
    }

    // Размер новых подматриц
    int new_size = n / 2;

    // Создаем подматрицы
    vector<vector<int>> A11(new_size, vector<int>(new_size));
    vector<vector<int>> A12(new_size, vector<int>(new_size));
    vector<vector<int>> A21(new_size, vector<int>(new_size));
    vector<vector<int>> A22(new_size, vector<int>(new_size));

    vector<vector<int>> B11(new_size, vector<int>(new_size));
    vector<vector<int>> B12(new_size, vector<int>(new_size));
    vector<vector<int>> B21(new_size, vector<int>(new_size));
    vector<vector<int>> B22(new_size, vector<int>(new_size));

    // Заполняем подматрицы
    split_matrix(A, A11, 0, new_size, 0, new_size);
    split_matrix(A, A12, 0, new_size, new_size, n);
    split_matrix(A, A21, new_size, n, 0, new_size);
    split_matrix(A, A22, new_size, n, new_size, n);

    split_matrix(B, B11, 0, new_size, 0, new_size);
    split_matrix(B, B12, 0, new_size, new_size, n);
    split_matrix(B, B21, new_size, n, 0, new_size);
    split_matrix(B, B22, new_size, n, new_size, n);

    // Вычисляем промежуточные матрицы по Штрассену
    vector<vector<int>> M1 = strassen_multiply(m_plus(A11, A22), m_plus(B11, B22));
    vector<vector<int>> M2 = strassen_multiply(m_plus(A21, A22), B11);
    vector<vector<int>> M3 = strassen_multiply(A11, m_minus(B12, B22));
    vector<vector<int>> M4 = strassen_multiply(A22, m_minus(B21, B11));
    vector<vector<int>> M5 = strassen_multiply(m_plus(A11, A12), B22);
    vector<vector<int>> M6 = strassen_multiply(m_minus(A21, A11), m_plus(B11, B12));
    vector<vector<int>> M7 = strassen_multiply(m_minus(A12, A22), m_plus(B21, B22));

    // Вычисляем блоки результирующей матрицы
    vector<vector<int>> C11 = m_plus(m_minus(m_plus(M1, M4), M5), M7);
    vector<vector<int>> C12 = m_plus(M3, M5);
    vector<vector<int>> C21 = m_plus(M2, M4);
    vector<vector<int>> C22 = m_plus(m_minus(m_plus(M1, M3), M2), M6);

    // Объединяем блоки в результирующую матрицу
    vector<vector<int>> C(n, vector<int>(n));
    join_matrix(C, C11, 0, new_size, 0, new_size);
    join_matrix(C, C12, 0, new_size, new_size, n);
    join_matrix(C, C21, new_size, n, 0, new_size);
    join_matrix(C, C22, new_size, n, new_size, n);

    return C;
}

// Вспомогательная функция для проверки, является ли размер степенью двойки
bool is_power_of_two(int n) {
    return (n & (n - 1)) == 0;
}

// Функция для дополнения матрицы до размера, являющегося степенью двойки
vector<vector<int>> pad_matrix(const vector<vector<int>>& matrix, int new_size) {
    int old_size = matrix.size();
    vector<vector<int>> new_matrix(new_size, vector<int>(new_size, 0));
    for (int i = 0; i < old_size; i++) {
        for (int j = 0; j < old_size; j++) {
            new_matrix[i][j] = matrix[i][j];
        }
    }
    return new_matrix;
}

// Функция для удаления дополнения из матрицы
vector<vector<int>> unpad_matrix(const vector<vector<int>>& matrix, int original_size) {
    vector<vector<int>> result(original_size, vector<int>(original_size));
    for (int i = 0; i < original_size; i++) {
        for (int j = 0; j < original_size; j++) {
            result[i][j] = matrix[i][j];
        }
    }
    return result;
}

// Основная функция умножения матриц с обработкой произвольных размеров
vector<vector<int>> multiply_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int n = A.size();

    // Проверка, что матрицы квадратные и одного размера
    if (A[0].size() != n || B.size() != n || B[0].size() != n) {
        throw invalid_argument("Matrices must be square and of the same size");
    }

    // Если размер является степенью двойки, просто умножаем
    if (is_power_of_two(n)) {
        return strassen_multiply(A, B);
    }

    // Иначе дополняем матрицы до ближайшей степени двойки
    int new_size = 1;
    while (new_size < n) {
        new_size <<= 1;
    }

    vector<vector<int>> A_padded = pad_matrix(A, new_size);
    vector<vector<int>> B_padded = pad_matrix(B, new_size);

    vector<vector<int>> C_padded = strassen_multiply(A_padded, B_padded);

    return unpad_matrix(C_padded, n);
}

// Функция для вывода матрицы
void print_matrix(const vector<vector<int>>& matrix) {
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    int size;
    cout << "Введите размер квадратных матриц: ";
    while (!(cin >> size) || size <= 0) {
        cout << "Ошибка! Пожалуйста, введите положительное целое число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    vector<vector<int>> A(size, vector<int>(size));
    vector<vector<int>> B(size, vector<int>(size));
    cout << endl << "Ввод матрицы A:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << "A[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> A[i][j];
        }
    }
    cout << endl << "Ввод матрицы B:" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << "B[" << i + 1 << "][" << j + 1 << "] = ";
            cin >> B[i][j];
        }
    }
    cout << endl << "Матрица A:" << endl;
    print_matrix(A);
    cout << endl << "Матрица B:" << endl;
    print_matrix(B);
    vector<vector<int>> C = multiply_matrices(A, B);
    cout << endl << "Результат умножения (матрица C):" << endl;
    print_matrix(C);
   
    return 0;
}