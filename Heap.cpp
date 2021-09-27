#include "CDA.cpp"
#include <cstdlib>
#include <iostream>
using namespace std;

template<typename keytype> class Heap {
    private:
        CDA<keytype> heap;
        int size;
    public:
        Heap(){
            size = 0;
        }
        Heap(Heap<keytype> &heap2) {
            heap = heap2.heap;
            size = heap2.size;
        }
        Heap<keytype>& operator=(Heap<keytype> &heap2) {
            heap = heap2.heap;
            size = heap2.size;
            return *this;
        }
        ~Heap() {
            for (int i = 0; i < heap.Length(); i++) {
                heap.DelEnd();
            }
        }
        int parent(int i) {
            return ((i-1)/2);
        }
        int left(int i) {
            return (2*i)+1;
        }
        int right(int i) {
            return (2*i)+2;
        }
        void heapify(CDA<keytype>& a, int i) {
            int l = left(i);
            int r = right(i);
            int min = i;
            if (l < a.Length() && a[l] < a[min]) {
                min = l;
            }
            else {
                min = i;
            }
            if (r < a.Length() && a[r] < a[min]) {
                min = r;
            }
            if (min != i) {
                keytype temp = a[i];
                a[i] = a[min];
                a[min] = temp;
                heapify(a, min);
            }
        }
        Heap(keytype k[], int i) {
            for (int x = 0; x < i; x++) {
                heap.AddEnd(k[x]);
            }
            size = heap.Length();
            for (int x = size/2; x >= 0; x--) {
                heapify(heap, x);
            }
        }
        keytype peekKey() {
            return heap[0];
        }
        keytype extractMin() {
            keytype min = heap[0];
            heap[0] = heap[size-1];
            heap.DelEnd();
            size--;
            heapify(heap, 0);
            return min;
        }
        void insertFix(int i) {
            if (i != 0 && heap[parent(i)] > heap[i]) {
                keytype temp = heap[i];
                heap[i] = heap[parent(i)];
                heap[parent(i)] = temp;
                insertFix(parent(i));
            }
        }
        void insert(keytype key) {
            heap.AddEnd(key);
            size++;
            insertFix(heap.Length()-1);
        }
        void printKey() {
            for (int i = 0; i < heap.Length(); i++) {
                cout << heap[i] << " ";
            }
            cout << endl;
        }
};