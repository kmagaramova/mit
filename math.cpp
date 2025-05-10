#include <iostream>
#include <string>
#include <stack>
using namespace std;

struct tree {
    char inf;
    tree* right;
    tree* left;
    tree* parent;
};

tree* node(char x) { 
    tree* n = new tree;
    n->inf = x;
    n->left = n->right = NULL;
    n->parent = NULL;
    return n;
}

tree* create_tree(string str) {
    tree* tr = NULL;
    for (unsigned int i = 0; i < str.length(); i++) {
        tree* n = node(str[i]);
        if (str[i] == '-' || str[i] == '+') {
            if (tr) {
                tr->parent = n;
                n->left = tr;
            }
            tr = n;
        }
        else if (str[i] == '/' || str[i] == '*') {
            if (!tr || isdigit(tr->inf)) {
                if (tr) tr->parent = n;
                n->left = tr;
                tr = n;
            }
            else {
                n->parent = tr;
                n->left = tr->right;;
                if (tr->right) tr->right->parent = n;
                tr->right = n;
            }
        }
        else {
            if (!tr) tr = n;
            else {
                if (!tr->right) {
                    n->parent = tr;
                    tr->right = n;
                }
                else {
                    tree* x = tr->right;
                    while (x->right) x = x->right;
                    n->parent = x;
                    x->right = n;
                }
            }
        }
    }
    return tr;
}

void preorder(tree* tr) {
    if (tr) {
        cout << tr->inf << " ";
        preorder(tr->left);
        preorder(tr->right);
    }
}

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
    getline(cin, str);
    tree* tr = create_tree(str);
    cout << "Префиксная форма: ";
    preorder(tr);
    cout << endl<<"Постфиксная форма: ";
    postorder(tr);
    return 0;
}