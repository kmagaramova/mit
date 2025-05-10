#include <iostream>
using namespace std;

struct Tree {
    int data;
    char color; // 'r' - красный, 'b' - черный
    Tree* left, * right, * parent;

    Tree(int val) : data(val), color('r'), left(nullptr), right(nullptr), parent(nullptr) {}
};

Tree* root = nullptr;

// Вспомогательные функции
Tree* grandparent(Tree* x) {
    if (x && x->parent)
        return x->parent->parent;
    return nullptr;
}

Tree* uncle(Tree* x) {
    Tree* g = grandparent(x);
    if (!g) return nullptr;
    return (x->parent == g->left) ? g->right : g->left;
}

Tree* sibling(Tree* x) {
    if (!x->parent) return nullptr;
    return (x == x->parent->left) ? x->parent->right : x->parent->left;
}

// Повороты
void left_rotate(Tree*& tr, Tree* x) {
    Tree* y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;

    if (!x->parent)
        tr = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

void right_rotate(Tree*& tr, Tree* x) {
    Tree* y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;

    if (!x->parent)
        tr = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;

    y->right = x;
    x->parent = y;
}

// Функции балансировки при вставке
void insert_case_1(Tree*& tr, Tree* x);
void insert_case_2(Tree*& tr, Tree* x);
void insert_case_3(Tree*& tr, Tree* x);
void insert_case_4(Tree*& tr, Tree* x);
void insert_case_5(Tree*& tr, Tree* x);

void insert_case_1(Tree*& tr, Tree* x) {
    if (!x->parent) {
        x->color = 'b';
    }
    else {
        insert_case_2(tr, x);
    }
}

void insert_case_2(Tree*& tr, Tree* x) {
    if (x->parent->color == 'b') {
        return;
    }
    else {
        insert_case_3(tr, x);
    }
}

void insert_case_3(Tree*& tr, Tree* x) {
    Tree* u = uncle(x);
    Tree* g;

    if (u && u->color == 'r') {
        x->parent->color = 'b';
        u->color = 'b';
        g = grandparent(x);
        g->color = 'r';
        insert_case_1(tr, g);
    }
    else {
        insert_case_4(tr, x);
    }
}

void insert_case_4(Tree*& tr, Tree* x) {
    Tree* g = grandparent(x);

    if (x == x->parent->right && x->parent == g->left) {
        left_rotate(tr, x->parent);
        x = x->left;
    }
    else if (x == x->parent->left && x->parent == g->right) {
        right_rotate(tr, x->parent);
        x = x->right;
    }
    insert_case_5(tr, x);
}

void insert_case_5(Tree*& tr, Tree* x) {
    Tree* g = grandparent(x);

    x->parent->color = 'b';
    g->color = 'r';
    if (x == x->parent->left && x->parent == g->left) {
        right_rotate(tr, g);
    }
    else {
        left_rotate(tr, g);
    }
}

// Вставка нового узла
void insert(Tree*& tr, Tree* prev, int x) {
    if (!tr) {
        Tree* n = new Tree(x);
        n->parent = prev;
        tr = n;
        insert_case_1(root, tr);
    }
    else {
        if (x < tr->data) {
            insert(tr->left, tr, x);
        }
        else if (x > tr->data) {
            insert(tr->right, tr, x);
        }
    }
}

// Вывод дерева
void printTree(Tree* tr, int level = 0) {
    if (tr) {
        printTree(tr->right, level + 1);
        for (int i = 0; i < level; i++) cout << "   ";
        cout << tr->data << (tr->color == 'r' ? "(r)" : "(b)") << endl;
        printTree(tr->left, level + 1);
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    int n, val;

    cout << "Введите количество элементов: ";
    cin >> n;

    cout << "Введите " << n << " элементов через пробел или с новой строки:\n";
    for (int i = 0; i < n; i++) {
        cin >> val;
        insert(root, nullptr, val);
    }

    cout << "\nКрасно-черное дерево:\n";
    printTree(root);

    return 0;
}