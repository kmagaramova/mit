#include <iostream>
#include <vector>
#include <set>
#include <stack>
#include <queue>

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

bool isEulerian(const vector<set<int>>& adjList, vector<int>& degrees) {
    int oddCount = 0;
    for (int i = 0; i < adjList.size(); ++i) {
        degrees[i] = adjList[i].size();
        if (degrees[i] % 2 != 0) {
            oddCount++;
        }
    }
    return (oddCount == 0); // Эйлеров цикл существует, если все степени четные
}

void findEulerianCycle(vector<set<int>> adjList) {
    int n = adjList.size();
    vector<int> degrees(n, 0);

    if (!isEulerian(adjList, degrees)) {
        cout << "Эйлеров цикл не существует (не все вершины имеют четную степень)" << endl;
        return;
    }

    stack<int> st;
    vector<int> cycle;
    st.push(0); // Начинаем с вершины 0

    while (!st.empty()) {
        int v = st.top();
        if (!adjList[v].empty()) {
            int u = *adjList[v].begin();
            st.push(u);
            adjList[v].erase(u);
            adjList[u].erase(v);
        }
        else {
            cycle.push_back(v);
            st.pop();
        }
    }

    cout << "Эйлеров цикл: ";
    for (int i = cycle.size() - 1; i >= 0; --i) {
        cout << cycle[i];
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "RUS");
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

    findEulerianCycle(adjList);

    return 0;
}