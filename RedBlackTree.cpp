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

// 1. Функция вставки нового элемента
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

// 2. Функция создания дерева
void createTree() {
    int n, val;
    cout << "Введите количество элементов: ";
    cin >> n;

    cout << "Введите " << n << " элементов:\n";
    for (int i = 0; i < n; i++) {
        cin >> val;
        insert(root, nullptr, val);
    }
}

// 3. Функция симметричного обхода (inorder)
void inorder(Tree* tr) {
    if (tr) {
        inorder(tr->left);
        cout << tr->data << (tr->color == 'r' ? "(r) " : "(b) ");
        inorder(tr->right);
    }
}

// 4. Функция поиска узла
Tree* findNode(Tree* tr, int val) {
    if (!tr) return nullptr;
    if (val == tr->data)
        return tr;
    else if (val < tr->data)
        return findNode(tr->left, val);
    else
        return findNode(tr->right, val);
}

// 5. Функции для удаления элемента
void deleteFix(Tree* x);
void transplant(Tree* u, Tree* v);

void deleteNode(Tree* z) {
    Tree* y = z;
    Tree* x;
    char y_original_color = y->color;

    if (!z->left) {
        x = z->right;
        transplant(z, z->right);
    }
    else if (!z->right) {
        x = z->left;
        transplant(z, z->left);
    }
    else {
        y = z->right;
        while (y->left) y = y->left;
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x) x->parent = y;
        }
        else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == 'b' && x)
        deleteFix(x);

    delete z;
}

void transplant(Tree* u, Tree* v) {
    if (!u->parent)
        root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;

    if (v)
        v->parent = u->parent;
}

void deleteFix(Tree* x) {
    while (x != root && (!x || x->color == 'b')) {
        if (x == x->parent->left) {
            Tree* s = x->parent->right;
            if (s && s->color == 'r') {
                s->color = 'b';
                x->parent->color = 'r';
                left_rotate(root, x->parent);
                s = x->parent->right;
            }

            if ((!s->left || s->left->color == 'b') &&
                (!s->right || s->right->color == 'b')) {
                s->color = 'r';
                x = x->parent;
            }
            else {
                if (!s->right || s->right->color == 'b') {
                    if (s->left) s->left->color = 'b';
                    s->color = 'r';
                    right_rotate(root, s);
                    s = x->parent->right;
                }

                s->color = x->parent->color;
                x->parent->color = 'b';
                if (s->right) s->right->color = 'b';
                left_rotate(root, x->parent);
                x = root;
            }
        }
        else {
            Tree* s = x->parent->left;
            if (s && s->color == 'r') {
                s->color = 'b';
                x->parent->color = 'r';
                right_rotate(root, x->parent);
                s = x->parent->left;
            }

            if ((!s->right || s->right->color == 'b') &&
                (!s->left || s->left->color == 'b')) {
                s->color = 'r';
                x = x->parent;
            }
            else {
                if (!s->left || s->left->color == 'b') {
                    if (s->right) s->right->color = 'b';
                    s->color = 'r';
                    left_rotate(root, s);
                    s = x->parent->left;
                }

                s->color = x->parent->color;
                x->parent->color = 'b';
                if (s->left) s->left->color = 'b';
                right_rotate(root, x->parent);
                x = root;
            }
        }
    }
    if (x) x->color = 'b';
}

// 6. Функция вывода всех листьев
void printLeaves(Tree* tr) {
    if (!tr) return;

    if (!tr->left && !tr->right) {
        cout << tr->data << (tr->color == 'r' ? "(r) " : "(b) ");
        return;
    }

    printLeaves(tr->left);
    printLeaves(tr->right);
}

// 7. Функция вывода дерева(изображение будет повернуто так, что корень будет слева, а дети будут уходить вправо)
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
    int choice, val;

    do {
        cout << "\nМеню:\n";
        cout << "1. Создать дерево\n";
        cout << "2. Добавить элемент\n";
        cout << "3. Удалить элемент\n";
        cout << "4. Симметричный обход\n";
        cout << "5. Вывести все листья\n";
        cout << "6. Вывести дерево\n";
        cout << "0. Выход\n";
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1:
            root = nullptr;
            createTree();
            break;
        case 2:
            cout << "Введите значение для вставки: ";
            cin >> val;
            insert(root, nullptr, val);
            break;
        case 3:
            cout << "Введите значение для удаления: ";
            cin >> val;
            {
                Tree* node = findNode(root, val);
                if (node)
                    deleteNode(node);
                else
                    cout << "Элемент не найден!\n";
            }
            break;
        case 4:
            cout << "Симметричный обход: ";
            inorder(root);
            cout << endl;
            break;
        case 5:
            cout << "Все листья: ";
            printLeaves(root);
            cout << endl;
            break;
        case 6:
            cout << "Дерево:\n";
            printTree(root);
            break;
        case 0:
            break;
        default:
            cout << "Неверный выбор!\n";
        }
    } while (choice != 0);

    return 0;
}