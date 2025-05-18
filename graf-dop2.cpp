#include <iostream>
#include <vector>
#include <set>
#include <stack>

using namespace std;

void printAdjList(const vector<set<int>>& adjList) {
    cout << "\nТекущий список смежности:\n";
    for (int i = 0; i < adjList.size(); ++i) {
        cout << i << ": ";
        for (int neighbor : adjList[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

// Вспомогательная рекурсивная функция для топологической сортировки
bool topologicalSortUtil(int v, const vector<set<int>>& adjList,
    vector<bool>& visited, vector<bool>& recStack,
    stack<int>& result) {
    visited[v] = true;
    recStack[v] = true;

    // Рекурсивно обрабатываем всех соседей текущей вершины
    for (int neighbor : adjList[v]) {
        if (!visited[neighbor]) {
            // Если соседняя вершина не посещена, обрабатываем ее
            if (!topologicalSortUtil(neighbor, adjList, visited, recStack, result))
                return false; // Обнаружен цикл
        }
        else if (recStack[neighbor]) {
            // Если соседняя вершина уже в текущем рекурсивном стеке - найден цикл
            return false;
        }
    }

    recStack[v] = false;
    result.push(v);
    return true;
}

// Основная функция топологической сортировки
bool topologicalSort(const vector<set<int>>& adjList, vector<int>& sorted) {
    int n = adjList.size();
    vector<bool> visited(n, false);  
    vector<bool> recStack(n, false);
    stack<int> result;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            // Если вершина не посещена, начинаем с нее обход
            if (!topologicalSortUtil(i, adjList, visited, recStack, result))
                return false; // Обнаружен цикл
        }
    }

    while (!result.empty()) {
        sorted.push_back(result.top());
        result.pop();
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "RUS");

    // Ввод графа
    int n, m;
    cout << "Введите количество вершин и рёбер: ";
    cin >> n >> m;

    vector<set<int>> adjList(n); 

    cout << "Введите " << m << " рёбер (пары вершин):\n";
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].insert(v);
    }

    printAdjList(adjList); 

    vector<int> sorted;
    if (topologicalSort(adjList, sorted)) {
        // Если сортировка успешна (граф ациклический)
        cout << "\nТопологическая сортировка:\n";
        for (int v : sorted) {
            cout << v << " ";
        }
        cout << endl;
    }
    else {
        // Если граф содержит циклы
        cout << "\nГраф содержит циклы, топологическая сортировка невозможна\n";
    }

    cout << endl;
    return 0;
}