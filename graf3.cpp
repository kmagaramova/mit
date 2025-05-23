﻿//Дан неориентированный граф. Подсчитать степень каждой вершины.
#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <queue>

using namespace std;

void printAdjList(const vector<set<int>>& adjList) {//вывод списка смежности
    cout << "\nТекущий список смежности:\n";
    for (int i = 0; i < adjList.size(); ++i) {
        cout << i << ": ";
        for (int neighbor : adjList[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }
}

void DFS(int n, vector<set<int>> adjList, int start) {//обход в глубину
    vector<bool> visited(n, false);
    stack<int> s;
    s.push(start);

    cout << "DFS: ";
    while (!s.empty()) {
        int v = s.top();
        s.pop();
        if (!visited[v]) {
            visited[v] = true;
            cout << v << " ";
            for (auto it = adjList[v].rbegin(); it != adjList[v].rend(); ++it) {
                if (!visited[*it]) s.push(*it);
            }
        }
    }
    cout << endl;
}

void BFS(int n, vector<set<int>> adjList, int start) {//обход в ширину
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    cout << "BFS: ";
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        cout << v << " ";
        for (int neighbor : adjList[v]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

void printVertexDegrees(const vector<set<int>>& adjList) {//Подсчитать степень каждой вершины.
    cout << "\nСтепени вершин (с учётом петель):\n";
    for (int i = 0; i < adjList.size(); ++i) {
        int degree = adjList[i].size();
        // Проверяем, есть ли петля (вершина соединена сама с собой)
        if (adjList[i].count(i)) {
            degree += 1; // Петля даёт +2 к степени, но в set она учтена как 1 элемент
        }
        cout << "Вершина " << i << ": степень " << degree << endl;
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    //ввод со списком смежности
    //int n;
    //cout << "Введите количество вершин: ";
    //cin >> n;

    //vector<set<int>> adjList(n);

    //cout << "Введите количество соседей и самих соседей для каждой вершины:\n";
    //for (int i = 0; i < n; ++i) {
    //    int k;
    //    cout << "Вершина " << i << ". Количество соседей: ";
    //    cin >> k;

    //    if (k > 0) {
    //        cout << "Соседи вершины " << i << ": ";
    //        for (int j = 0; j < k; ++j) {
    //            int neighbor;
    //            cin >> neighbor;
    //            if (neighbor < 0 || neighbor >= n) {
    //                cout << "Ошибка! Неверный номер вершины. Повторите ввод.\n";
    //                --j; // Повторяем ввод для этого соседа
    //                continue;
    //            }
    //            adjList[i].insert(neighbor);
    //        }
    //    }
    //}

//ввод с ребрами
    int n, m;
    cout << "Введите количество вершин и рёбер: ";
    cin >> n >> m;

    vector<set<int>> adjList(n);

    cout << "Введите " << m << " рёбер (пары вершин):\n";
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].insert(v);
        adjList[v].insert(u);
    }
    printAdjList(adjList);
    //задание 3
    printVertexDegrees(adjList);
    /*int start;
    cout << "Стартовая вершина для BFS и DFS: ";
    cin >> start;
    DFS(n, adjList, start);
    BFS(n, adjList, start);*/
    return 0;
}