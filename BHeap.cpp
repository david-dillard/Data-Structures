#include <cstdlib>
#include <iostream>
using namespace std;

template<typename keytype> class Node {
    public: 
        Node<keytype>* parent;
        Node<keytype>* child;
        Node<keytype>* sibling;
        keytype key;
        int degree;
    
        Node() {
            parent = nullptr;
            child = nullptr;
            sibling = nullptr;
            degree = 0;
        }
        void preorderRecur(Node<keytype>* x) {
            if (x == nullptr) {
                return;
            }
            if (x->parent == nullptr) {
                cout << "B" << x->degree << endl;
                cout << x->key << " ";
                preorderRecur(x->child);
            }
            else {
                cout << x->key << " ";
                preorderRecur(x->child);
                preorderRecur(x->sibling);
            }
        }
};

template<typename keytype> class BHeap {
    private:
        Node<keytype>* min;
        Node<keytype>* head;
    public:
        BHeap() {
            min = nullptr;
            head = nullptr;
        }
        BHeap(keytype k[], int s) {
            head = new Node<keytype>();
            head->child = head->parent = head->sibling = nullptr;
            head->degree = 0;
            head->key = k[0];
            min = head;
            for (int i = 1; i < s; i++) {
                insert(k[i]);
            }
        }
        Node<keytype>* copyTree(Node<keytype>* x, Node<keytype>* p) {
            Node<keytype>* copy;
            if (x != nullptr) {
                copy = new Node<keytype>();
                copy->key = x->key;
                copy->child = copy->sibling = nullptr;
                copy->parent = p;
                copy->child = copyTree(x->child, copy);
                copy->sibling = copyTree(x->sibling, copy);
                return copy;
            }
            return nullptr;
        }
        BHeap(BHeap<keytype> &heap2) {
            head = new Node<keytype>();
            head->key = heap2.head->key;
            head->degree = heap2.head->degree;
            head->sibling = new Node<keytype>();
            Node<keytype>* cur1 = head->sibling;
            for (Node<keytype>* cur = heap2.head->sibling; cur != nullptr; cur = cur->sibling ) {
                cur1->key = cur->key;
                cur1->parent = nullptr;
                cur1->degree = cur->degree;
                cur1->child = copyTree(cur->child, cur);
                cur1->sibling = new Node<keytype>();
                cur1 = cur1->sibling;
            }
            min = findNewMin(head);
        }
        BHeap<keytype>& operator=(BHeap<keytype>& heap2) {
            if (this != &heap2) {
                delete this;
                head->key = heap2.head->key;
                head->sibling = new Node<keytype>();
                Node<keytype>* cur1 = head->sibling;
                for (Node<keytype>* cur = heap2.head->sibling; cur != nullptr; cur = cur->sibling ) {
                    cur1->key = cur->key;
                    cur1->parent = nullptr;
                    cur1->degree = cur->degree;
                    cur1->child = copyTree(cur->child, cur);
                    cur1->sibling = new Node<keytype>();
                    cur1 = cur1->sibling;
                }
                min = findNewMin(head);
            }
            return *this;
        }
        void deleteHeap(Node<keytype>* x) {
            if (x == nullptr) {
                return;
            }
            deleteHeap(x->child);
            deleteHeap(x->sibling);
            x = nullptr;
        }
        ~BHeap() {
            Node<keytype>* cur = head;
            while(cur != nullptr) {
                deleteHeap(cur->child);
                cur = cur->sibling;
            }
        }
        keytype peekKey() {
            return min->key;
        }
        keytype extractMin() {
            keytype minVal = min->key;
            
            Node<keytype>* tempPrev = head;
            while (1) {
                if (tempPrev->sibling == nullptr || tempPrev->sibling == min) {
                    break;
                }
                tempPrev = tempPrev->sibling;
            }
            Node<keytype>* temp = min;
            if (temp == head) {
                head = temp->sibling;
            }
            else {
                tempPrev->sibling = temp->sibling;
            }
            Node<keytype>* newHead = nullptr;
            Node<keytype>* child = temp->child;
            while(child != nullptr) {
                Node<keytype>* next = child->sibling;
                child->sibling = newHead;
                child->parent = nullptr;
                newHead = child;
                child = next;
            }
            if (min->degree == 0) {
                return minVal;
            }
            min = head;
            BHeap<keytype>* newHeap = new BHeap<keytype>();
            newHeap->head = newHead;
            newHeap->min = findNewMin(newHeap->head);
            merge(*newHeap);
            return minVal;
        }
        Node<keytype>* findNewMin(Node<keytype>* h) {
            Node<keytype>* x = h;
            Node<keytype>* newMin = h;
            while(x != nullptr) {
                if (x->key < newMin->key) {
                    newMin = x;
                }
                x = x->sibling;
            }
            return newMin;
        }
        void mergeTrees(Node<keytype>* x, Node<keytype>* y) {
            x->parent = y;
            x->sibling = y->child;
            y->child = x;
            y->degree++;
        }
        BHeap<keytype>* linkLists(Node<keytype>* head2, Node<keytype>* min2) {
            Node<keytype>* newHead = nullptr;
            Node<keytype>* cur1 = head;
            Node<keytype>* cur2 = head2;
            Node<keytype>* temp = nullptr;
            if (cur1 != nullptr) {
                if (cur2 != nullptr) {
                    if (cur1->degree <= cur2->degree) {
                        newHead = cur1;
                    }
                    else if (cur1->degree > cur2->degree){
                        newHead = cur2;
                    }
                }
                else {
                    newHead = cur1;
                }
            }
            else{
                newHead = cur2;
            }

            while (cur1 != nullptr && cur2 != nullptr) {
                if(cur1->degree <= cur2->degree) {
                    temp = cur1->sibling;
                    cur1->sibling = cur2;
                    cur1 = temp;
                }
                else {
                    temp = cur2->sibling;
                    cur2->sibling = cur1;
                    cur2 = temp;
                }
            }
            BHeap<keytype>* newHeap = new BHeap<keytype>();
            if (min->key > min2->key) {
                newHeap->min = min2;
            }
            else {
                newHeap->min = min;
            }
            head2 = nullptr;
            newHeap->head = newHead;
            return newHeap;
        }
        void merge(BHeap<keytype> &H2) {
            if (H2.head == nullptr) {
                return;
            }
            BHeap<keytype>* newHeap = new BHeap<keytype>();
            newHeap = linkLists(H2.head, H2.min);
            Node<keytype>* prevX = nullptr;
            Node<keytype>* x = newHeap->head; 
            Node<keytype>* nextX = x->sibling;
            while(nextX != nullptr) {
                if (x->degree != nextX->degree || (nextX->sibling != nullptr && nextX->sibling->degree == x->degree)) {
                    prevX = x;
                    x = nextX;
                }
                else {
                    if (x->key <= nextX->key) {
                        x->sibling = nextX->sibling;
                        mergeTrees(nextX, x);
                    }
                    else {
                        if(prevX == nullptr) {
                            newHeap->head = nextX;
                        }
                        else {
                            prevX->sibling = nextX;
                        }
                        mergeTrees(x, nextX);
                        x = nextX;
                    }
                }
                nextX = x->sibling;
            }
            head = newHeap->head;
            min = newHeap->min;
        }
        void insert(keytype k) {
            if (head == nullptr) {
                head = new Node<keytype>();
                head->child = head->parent = head->sibling = nullptr;
                head->degree = 0;
                head->key = k;
                min = head;
                return;
            }
            BHeap<keytype>* newHeap = new BHeap<keytype>();
            Node<keytype>* x = new Node<keytype>();
            x->key = k;
            x->parent = nullptr;
            x->child = nullptr;
            x->sibling = nullptr;
            x->degree = 0;
            newHeap->head = x;
            newHeap->min = x;
            merge(*newHeap);
        }
        void printKey() {
            Node<keytype>* cur = head;
            while(cur != nullptr) {
                cur->preorderRecur(cur);
                cout << endl << endl;
                cur = cur->sibling;
            }
        }

};