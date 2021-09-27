#include <cstdlib>
#include <iostream>
using namespace std;

enum Colors {BLACK, RED};

template <class keytype, class valuetype> class node {
public:
    keytype key;
    valuetype value;
    node<keytype, valuetype>* left;
    node<keytype, valuetype>* right;
    node<keytype, valuetype>* parent;
    Colors color;
    int size;


    void inorderRecur(node<keytype, valuetype>* x, node<keytype, valuetype>* nil){
        if (x == nil) {
            return;
        }
        inorderRecur(x->left, nil);
        cout << x->key << " ";
        inorderRecur(x->right, nil);
    }
    void preorderRecur(node<keytype, valuetype>* x, node<keytype, valuetype>* nil) {
        if (x == nil) {
            return;
        }
        cout << x->key << " ";
        preorderRecur(x->left, nil);
        preorderRecur(x->right, nil);
    }
    void postorderRecur(node<keytype, valuetype>* x, node<keytype, valuetype>* nil) {
        if (x == nil) {
            return;
        }
        postorderRecur(x->left, nil);
        postorderRecur(x->right, nil);
        cout << x->key << " ";
    }
};

template <class keytype, class valuetype> class RBTree {
    private: 
        node<keytype, valuetype>* tree;
        node<keytype, valuetype>* nil;
    public:
        RBTree() {
            nil = new node<keytype, valuetype>();
            nil->left = nullptr;
            nil->right = nullptr;
            nil->color = BLACK;
            nil->size = 0;
            tree = nil;
        }
        RBTree(keytype k[], valuetype V[], int s) {
            nil = new node<keytype, valuetype>();
            nil->left = nullptr;
            nil->right = nullptr;
            nil->color = BLACK;
            nil->size = 0;
            tree = nil;
            for (int i = 0; i < s; i++) {
                insert(k[i], V[i]);
            }
        }
        RBTree(RBTree<keytype, valuetype>& tree2){
            nil = new node<keytype, valuetype>();
            nil->left = nullptr;
            nil->right = nullptr;
            nil->color = BLACK;
            nil->size = 0;
            tree = copyTree(tree2.tree, nil, tree2.nil);
        }
        RBTree<keytype, valuetype>& operator=(RBTree<keytype, valuetype>& tree2) {
            if (this != &tree2) {
                delete this;
                nil = new node<keytype, valuetype>();
                nil->left = nullptr;
                nil->right = nullptr;
                nil->color = BLACK;
                nil->size = 0;
                tree = copyTree(tree2->tree, nil, tree2->nil);
            }
            return *this;
        }
        node<keytype, valuetype>* copyTree(node<keytype, valuetype>* x, node<keytype, valuetype>* p, node<keytype, valuetype>* xNil) {
            node<keytype, valuetype>* copy;
            if (x != xNil) {
                copy = new node<keytype, valuetype>();
                copy->key = x->key; copy->value = x->value;
                copy->size = x->size; copy->color = x->color;
                copy->left = copy->right = nil;
                copy->parent = p;
                copy->left = copyTree(x->left, copy, xNil);
                copy->right = copyTree(x->right, copy, xNil);
                return copy;
            }
            return nil;
        }
        ~RBTree() {
            while(tree != nil) {
                remove(tree->key);
            }
            tree = nullptr;
        }
        valuetype* searchHelper(node<keytype, valuetype>* x, keytype k) {
            if (x == nil) {
                return NULL;
            }
            if (x->key == k) {
                return &(x->value);
            }
            if (x->key < k) {
                return searchHelper(x->right, k);
            }
            return searchHelper(x->left, k);
        }
        valuetype* search(keytype k) {
            return searchHelper(tree, k);
            
        }
        void insert(keytype k, valuetype v) {
            node<keytype, valuetype>* z = new node<keytype, valuetype>();
            z->parent = nullptr;
            z->key = k;
            z->value = v;
            z->size = 1;
            z->left = nil;
            z->right = nil;
            z->color = RED;
            node<keytype, valuetype>* y = nil;
            node<keytype, valuetype>* x = tree;
            while(x != nil) {
                y = x;
                if(z->key < x->key) {
                    x->size++;
                    x = x->left;
                }
                else {
                    x->size++;
                    x = x->right;
                }
            }
            z->parent = y;
            if (y == nil) {
                tree = z;
            }
            else if(z->key < y->key) {
                y->left = z;
            }
            else {
                y->right = z;
            }
            z->left = nil;
            z->right = nil;
            z->color = RED;
            insertFix(z);
        }
        void insertFix(node<keytype, valuetype>* z) {
            while(z->parent->color == RED) {
                if(z->parent == z->parent->parent->left) {
                    node<keytype, valuetype>* y = z->parent->parent->right;
                    if (y->color == RED) {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    }
                    else {
                        if(z == z->parent->right){
                            z = z->parent;
                            rotateLeft(z);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        rotateRight(z->parent->parent);
                    }
                }
                else {
                    node<keytype, valuetype>* y = z->parent->parent->left;
                    if (y->color == RED) {
                        z->parent->color = BLACK;
                        y->color = BLACK;
                        z->parent->parent->color = RED;
                        z = z->parent->parent;
                    }
                    else {
                        if(z == z->parent->left){
                            z = z->parent;
                            rotateRight(z);
                        }
                        z->parent->color = BLACK;
                        z->parent->parent->color = RED;
                        rotateLeft(z->parent->parent);
                    }
                }
            }
            tree->color = BLACK;
        }

        void keyChange(node<keytype, valuetype>* x, node<keytype, valuetype>* y) {
            if(x->parent == nil) {
                tree = y;
            }
            else if (x == x->parent->left) {
                x->parent->left = y;
            }
            else {
                x->parent->right = y;
            }
            y->parent = x->parent;
        }

        int remove(keytype k) {
            node<keytype, valuetype>* cur = tree;
            while(1) {
                if (k == cur->key) {
                    break;
                }
                if (k < cur->key) {
                    if (cur->left == nil) {
                        return 0;
                    }
                    cur = cur->left;
                }
                else {
                    if (cur->right == nil) {
                        return 0;
                    }
                    cur = cur->right;
                }
            }

            node<keytype, valuetype>* temp = cur;
            if (cur != tree) {
                    while(temp != tree) {
                        temp = temp->parent;
                        temp->size--;
                    }
                tree->size--;
            }
            node<keytype, valuetype>* y = cur;
            node<keytype, valuetype>* x = nil;
            Colors yCol = y->color;
            if(cur->left == nil) {
                x = cur->right;
                keyChange(cur, cur->right);
            }
            else if(cur->right == nil) {
                x = cur->left;
                keyChange(cur, cur->left);
            } 
            else {
                y = predecessorNode(cur->key);
                node<keytype, valuetype>* temp = y;
                yCol = y->color;
                x = y->left;
                if(y->parent == cur) {
                    x->parent = y;
                }
                else {
                    keyChange(y, y->left);
                    y->left = cur->left;
                    y->left->parent = y;
                    node<keytype, valuetype>* tmp = x->parent; 
                    while (tmp != y) { 
                        tmp->size--; 
                        tmp = tmp->parent; 
                    } 
                }
                keyChange(cur, y);
                y->right = cur->right;
                y->right->parent = y;
                y->color = cur->color;
                y->size = y->left->size + y->right->size + 1;
            }
            if (yCol == BLACK) {
                deleteFix(x);
            }
            return 1;
        }
        void rotateLeft(node<keytype, valuetype>* x) {
            node<keytype, valuetype>* y = x->right;
            x->right = y->left;
            if (x->right != nil) {
                x->right->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == nil) {
                tree = y;
            }
            else {
                if (x == x->parent->left) {
                    x->parent->left = y;
                }
                else {
                    x->parent->right = y;
                }
            }
            y->left = x;
            x->parent = y;
            y->size = x->size;
            x->size = x->left->size + x->right->size + 1;

        }
        void rotateRight(node<keytype, valuetype>* y) {
            node<keytype, valuetype>* x = y->left;
            y->left = x->right;
            if (x->right != nil) {
                x->right->parent = y;
            }
            x->parent = y->parent;
            if (y->parent == nil) {
                tree = x;
            }
            else { 
                if (y == y->parent->right) {
                    y->parent->right = x;
                }
                else {
                    y->parent->left = x;
                }
            }
            x->right = y;
            y->parent = x;
            x->size = y->size;
            y->size = y->left->size + y->right->size + 1;
        }
        void deleteFix(node<keytype, valuetype>* x) {
            while (x != tree && x->color == BLACK) {
                if (x == x->parent->left) {
                    node<keytype, valuetype>* w = x->parent->right;
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        rotateLeft(x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->color == BLACK && w->right->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    }
                    else {
                        if (w->right->color == BLACK) {
                            w->left->color = BLACK;
                            w->color = RED;
                            rotateRight(w);
                            w = x->parent->right;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->right->color = BLACK;
                        rotateLeft(x->parent);
                        x = tree;
                    }
                }
                else {
                    node<keytype, valuetype>* w = x->parent->left;
                    if (w->color == RED) {
                        w->color = BLACK;
                        x->parent->color = RED;
                        rotateRight(x->parent);
                        w = x->parent->left;
                    }
                    if (w->left->color == BLACK && w->right->color == BLACK) {
                        w->color = RED;
                        x = x->parent;
                    }
                    else {
                        if (w->left->color == BLACK) {
                            w->right->color = BLACK;
                            w->color = RED;
                            rotateLeft(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = BLACK;
                        w->left->color = BLACK;
                        rotateRight(x->parent);
                        x = tree;
                    }
                }
            }
            x->color = BLACK;
            tree->color = BLACK;
        }
        int rankHelp(node<keytype, valuetype>* x) {
            int rank = x->left->size + 1;
            node<keytype, valuetype>* cur = x;
            while (cur != tree) {
                if (cur == cur->parent->right) {
                    rank = rank + cur->parent->left->size + 1;
                }
                cur = cur->parent;
            }
            return rank;
        }

        int rank(keytype k) {
            node<keytype, valuetype>* cur = tree;
            while(1) {
                if (k == cur->key) {
                    break;
                }
                if (k < cur->key) {
                    if (cur->left == nil) {
                        return 0;
                    }
                    cur = cur->left;
                }
                else {
                    if (cur->right == nil) {
                        return 0;
                    }
                    cur = cur->right;
                }
            }
            return rankHelp(cur);
        }
        node<keytype, valuetype>* selectHelp(node<keytype, valuetype>* x, int pos) {

            int rank = x->left->size + 1;
            
            if (pos == rank) {
                return x;
            }
            else if (pos < rank) {
                return selectHelp(x->left, pos);
            }
            else {
                return selectHelp(x->right, pos - rank);
            }
        }
        keytype select(int pos) {
            return selectHelp(tree, pos)->key;
        }
        keytype* successor(keytype k) {
            node<keytype, valuetype>* cur = tree;
            while(1) {
                if (k == cur->key) {
                    break;
                }
                if (k < cur->key) {
                    if (cur->left == nil) {
                        return NULL;
                    }
                    cur = cur->left;
                }
                else {
                    if (cur->right == nil) {
                        return NULL;
                    }
                    cur = cur->right;
                }
            }
            if(cur->right != nil) {
                cur = cur->right;
                while(cur->left != nil) {
                    cur = cur->left;
                }
                return &(cur->key);
            }
            else {
                node<keytype, valuetype>* s = NULL;
                node<keytype, valuetype>* a = tree;
                while (a != cur) {
                    if(cur->key < a->key) {
                        s = a;
                        a = a->left;
                    }
                    else {
                        a = a->right;
                    }
                }

                return &(s->key);
            }
        }
        keytype* predecessor(keytype k) {
            node<keytype, valuetype>* cur = tree;
            while(1) {
                if (k == cur->key) {
                    break;
                }
                if (k < cur->key) {
                    if (cur->left == nil) {
                        return NULL;
                    }
                    cur = cur->left;
                }
                else {
                    if (cur->right == nil) {
                        return NULL;
                    }
                    cur = cur->right;
                }
            }
            if(cur->left != nil) {
                cur = cur->left;
                while(cur->right != nil) {
                    cur = cur->right;
                }
                return &(cur->key);
            }
            else {
                node<keytype, valuetype>* p = NULL;
                node<keytype, valuetype>* a = tree;
                while (a != cur) {
                    if(cur->key > a->key) {
                        p = a;
                        a = a->right;
                    }
                    else {
                        a = a->left;
                    }
                }

                return &(p->key);
            }
        }
        node<keytype, valuetype>* predecessorNode(keytype k) {
            node<keytype, valuetype>* cur = tree;
            while(1) {
                if (k == cur->key) {
                    break;
                }
                if (k < cur->key) {
                    if (cur->left == nil) {
                        return NULL;
                    }
                    cur = cur->left;
                }
                else {
                    if (cur->right == nil) {
                        return NULL;
                    }
                    cur = cur->right;
                }
            }
            if(cur->left != nil) {
                cur = cur->left;
                while(cur->right != nil) {
                    cur = cur->right;
                }
                return cur;
            }
            else {
                node<keytype, valuetype>* p = NULL;
                node<keytype, valuetype>* a = tree;
                while (a != cur) {
                    if(cur->key > a->key) {
                        p = a;
                        a = a->right;
                    }
                    else {
                        a = a->left;
                    }
                }

                return p;
            }
        }
        int size() {
            return tree->size;
        }
        void inorder() {
            tree->inorderRecur(tree, nil);
            cout << endl;
        }
        void preorder() {
            tree->preorderRecur(tree, nil);
            cout << endl;
        }
        void postorder() {
            tree->postorderRecur(tree, nil);
            cout << endl;
        }
        void printk(int k) {
            for (int i = 1; i <= k; i++) {
                cout << select(i) << " ";
            }
            cout << endl;
        }

};