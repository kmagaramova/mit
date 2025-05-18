﻿// Дан ориентированный граф. Вывести все истоки графа.
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

// Функция для проверки, является ли вершина истоком с помощью BFS
bool isSourceBFS(int v, const vector<set<int>>& adjList) {
    int n = adjList.size();
    for (int u = 0; u < n; ++u) {
        if (u == v) continue;

        vector<bool> visited(n, false);
        queue<int> q;
        q.push(u);
        visited[u] = true;

        bool found = false;
        while (!q.empty() && !found) {
            int current = q.front();
            q.pop();

            for (int neighbor : adjList[current]) {
                if (neighbor == v) {
                    found = true;
                    break;
                }
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        }

        if (found) return false;
    }
    return true;
}

// Функция для поиска всех истоков с использованием BFS
vector<int> findSourcesWithBFS(const vector<set<int>>& adjList) {
    vector<int> sources;
    for (int v = 0; v < adjList.size(); ++v) {
        if (isSourceBFS(v, adjList)) {
            sources.push_back(v);
        }
    }
    return sources;
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
    }

    // Поиск и вывод истоков с использованием BFS
    vector<int> sources = findSourcesWithBFS(adjList);

    cout << "\nИстоки графа ";
    if (sources.empty()) {
        cout << "истоков нет";
    }
    else {
        for (int source : sources) {
            cout << source << " ";
        }
    }
    cout << endl;
    /*int start;
   cout << "Стартовая вершина для BFS и DFS: ";
   cin >> start;
   DFS(n, adjList, start);
   BFS(n, adjList, start);*/
    return 0;
}