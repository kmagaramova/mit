#include <iostream>
#include <vector>
#include <queue>
#include <stack>  // Для DFS со стеком
using namespace std;

// DFS со стеком (итеративный)
void DFS(int startVertex, const vector<vector<int>>& adjList) {
    vector<bool> visited(adjList.size(), false);
    stack<int> s;

    s.push(startVertex);

    cout << "DFS: ";
    while (!s.empty()) {
        int vertex = s.top();
        s.pop();

        if (!visited[vertex]) {
            visited[vertex] = true;
            cout << vertex << " ";

            // Добавляем соседей в обратном порядке
            for (auto it = adjList[vertex].rbegin(); it != adjList[vertex].rend(); ++it) {
                if (!visited[*it]) {
                    s.push(*it);
                }
            }
        }
    }
    cout << endl;
}

// BFS (очередь)
void BFS(int startVertex, const vector<vector<int>>& adjList) {
    vector<bool> visited(adjList.size(), false);
    queue<int> q;

    visited[startVertex] = true;
    q.push(startVertex);

    cout << "BFS: ";
    while (!q.empty()) {
        int vertex = q.front();
        q.pop();
        cout << vertex << " ";

        for (int neighbor : adjList[vertex]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "RUS");
    int n, m;
    cout << "Введите количество вершин и рёбер: ";
    cin >> n >> m;

    vector<vector<int>> adjList(n);  // Нумерация с 0

    cout << "Введите рёбра (пары вершин):\n";
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adjList[u].push_back(v);
        adjList[v].push_back(u);  // Граф неориентированный
    }

    int startVertex;
    cout << "Введите стартовую вершину: ";
    cin >> startVertex;

    DFS(startVertex, adjList);  // DFS со стеком
    BFS(startVertex, adjList);        // BFS

    return 0;
}