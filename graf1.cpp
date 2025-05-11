#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <queue>

using namespace std;

void DFS(int n, vector<set<int>> adjList, int start) {
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

void BFS(int n, vector<set<int>> adjList, int start) {
    vector<bool> visited(n, false);
    visited.assign(n, false);
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

int main() {
    setlocale(LC_ALL, "RUS");
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
    int start;
    cout << "Стартовая вершина для BFS и DFS: ";
    cin >> start;
    DFS(n, adjList, start);
    BFS(n, adjList, start);
    return 0;
}