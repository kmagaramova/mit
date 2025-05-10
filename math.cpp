#include <iostream>
#include <string>
#include <stack>
using namespace std;

// Структура для представления узла бинарного дерева
struct tree {
    char inf;        // Информационное поле (символ)
    tree* right;     // Указатель на правое поддерево
    tree* left;      // Указатель на левое поддерево
    tree* parent;    // Указатель на родительский узел
};

// Функция создания нового узла дерева
tree* node(char x) {
    tree* n = new tree;
    n->inf = x;
    n->left = n->right = NULL;
    n->parent = NULL;
    return n;
}

// Функция построения дерева из строки (инфиксное выражение)
tree* create_tree(string str) {
    tree* tr = NULL;  // Корень дерева

    for (unsigned int i = 0; i < str.length(); i++) {
        tree* n = node(str[i]);  // Создаем новый узел для текущего символа

        // Обработка операторов + и -
        if (str[i] == '-' || str[i] == '+') {
            if (tr) {
                // Если дерево не пусто, делаем текущий корень левым потомком нового узла
                tr->parent = n;
                n->left = tr;
            }
            tr = n;  // Новый узел становится корнем
        }
        // Обработка операторов * и /
        else if (str[i] == '/' || str[i] == '*') {
            if (!tr || isdigit(tr->inf)) {
                // Если дерево пусто или в корне цифра, аналогично операторам + и -
                if (tr) tr->parent = n;
                n->left = tr;
                tr = n;
            }
            else {
                // Иначе добавляем узел в правое поддерево текущего корня
                n->parent = tr;
                n->left = tr->right;
                if (tr->right) tr->right->parent = n;
                tr->right = n;
            }
        }
        // Обработка операндов (цифр)
        else {
            if (!tr) tr = n;  // Если дерево пусто, узел становится корнем
            else {
                if (!tr->right) {
                    // Если нет правого поддерева, добавляем узел туда
                    n->parent = tr;
                    tr->right = n;
                }
                else {
                    // Иначе идем до конца правой ветви и добавляем узел
                    tree* x = tr->right;
                    while (x->right) x = x->right;
                    n->parent = x;
                    x->right = n;
                }
            }
        }
    }
    return tr;  // Возвращаем корень построенного дерева
}

// Префиксный обход дерева (корень -> левое поддерево -> правое поддерево)
void preorder(tree* tr) {
    if (tr) {
        cout << tr->inf << " ";
        preorder(tr->left);
        preorder(tr->right);
    }
}

// Постфиксный обход дерева (левое поддерево -> правое поддерево -> корень)
void postorder(tree* tr) {
    if (tr) {
        postorder(tr->left);
        postorder(tr->right);
        cout << tr->inf << " ";
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    string str;
    cout << "Введите в инфиксной форме выражение: ";
    getline(cin, str);  // Чтение введенного выражения

    tree* tr = create_tree(str);  // Построение дерева выражения

    cout << "Префиксная форма: ";
    preorder(tr);  // Вывод префиксной формы (прямой обход)

    cout << endl << "Постфиксная форма: ";
    postorder(tr);  // Вывод постфиксной формы (обратный обход)

    return 0;
}