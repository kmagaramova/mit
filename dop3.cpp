#include <iostream>
#include <vector>
#include <climits>
#include <stack>

using namespace std;
// Структура для хранения ребра графа
struct Edge {
    int to;//вершина
    int weight;//вес
    Edge(int t, int w) : to(t), weight(w) {}
};

void printAdjList(const vector<vector<Edge>>& adjList) {
    cout << "\nСписок смежности:\n";
    for (int i = 0; i < adjList.size(); ++i) {
        cout << i << ": ";
        for (const Edge& e : adjList[i]) {
            cout << "(" << e.to << ", " << e.weight << ") ";
        }
        cout << endl;
    }
}

// Находит индекс вершины с минимальным расстоянием, ещё не посещённой
int findMin(const vector<int>& dist, const vector<bool>& visited) {
    int minDist = INT_MAX;
    int minVertex = -1;
    for (int i = 0; i < dist.size(); ++i) {
        if (!visited[i] && dist[i] < minDist) {
            minDist = dist[i];
            minVertex = i;
        }
    }
    return minVertex;
}

void f1(const vector<vector<Edge>>& adjList, int start) {
    int n = adjList.size();
    vector<int> dist(n, INT_MAX);// Кратчайшие расстояния
    vector<int> prev(n, -1);// Для восстановления путей
    vector<bool> visited(n, false);// Посещённые вершины
    dist[start] = 0;
    for (int i = 0; i < n; ++i) {
        // Находим вершину с минимальным расстоянием
        int u = findMin(dist, visited);
        if (u == -1) break; // Все оставшиеся вершины недостижимы
        visited[u] = true;
        // Обновляем расстояния до соседей
        for (const Edge& e : adjList[u]) {
            int v = e.to;
            int weight = e.weight;
            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                prev[v] = u;
            }
        }
    }
    // Вывод результатов
    cout << "\nКратчайшие расстояния от вершины " << start << ":\n";
    for (int i = 0; i < n; ++i) {
        cout << "До вершины " << i << ": ";
        if (dist[i] == INT_MAX) {
            cout << "нет пути\n";
        }
        else {
            cout << "расстояние = " << dist[i] << ", путь: ";
            // Восстановление пути
            stack<int> path;
            for (int at = i; at != -1; at = prev[at]) {
                path.push(at);
            }
            // Вывод пути
            while (!path.empty()) {
                cout << path.top();
                path.pop();
                if (!path.empty()) cout << " -> ";
            }
            cout << endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    int n, m;
    cout << "Введите количество вершин и рёбер: ";
    cin >> n >> m;

    vector<vector<Edge>> adjList(n);

    cout << "Введите " << m << " рёбер (вершина1 вершина2 вес):\n";
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        adjList[u].emplace_back(v, w);
        adjList[v].emplace_back(u, w);
    }

    printAdjList(adjList);

    int start;
    cout << "\nСтартовая вершина: ";
    cin >> start;

    if (start < 0 || start >= n) {
        cout << "Ошибка! Неверный номер вершины.\n";
        return 1;
    }

    f1(adjList, start);

    return 0;
}