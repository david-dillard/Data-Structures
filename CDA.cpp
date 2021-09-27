#include <cstdlib>
#include <iostream>

using namespace std;

template<typename elmtype> class CDA {
    private:
        elmtype* array;
        int size;
        int capacity;
        int startIndex;
        int endIndex;
        bool isReverse = false;
    public:
        CDA() {
            array = new elmtype[1];
            capacity = 1;
            size = 0;
            startIndex = 0;
            endIndex = 0;
        }
        CDA(int s) {
            array = new elmtype[s];
            capacity = s;
            size = s;
            startIndex = 0;
            endIndex = s-1;
        }
        CDA(CDA<elmtype> &dArray2) {
            capacity = dArray2.Capacity();
            array = new elmtype[capacity];
            
            for (int i = 0; i < dArray2.Length(); i++) {
                array[i] = dArray2[i];
            }
            size = dArray2.Length();
            startIndex = 0;
            endIndex = size-1;
        }
        CDA<elmtype>& operator=(CDA<elmtype> &dArray2) {
            delete [] array;
            capacity = dArray2.Capacity();
            array = new elmtype[capacity];
            for (int i = 0; i < dArray2.Length(); i++) {
                array[i] = dArray2[i];
            }
            size = dArray2.Length();
            startIndex = 0;
            endIndex = size-1;
            return *this;
        }
        ~CDA() {
            delete [] array;
            size = 0;
            capacity = 0;
            startIndex = 0;
            endIndex = 0;
        }
        elmtype& operator[](int i){
            if(i > size || i < 0) {
                cout << "Index is out of bounds." << endl;
                return array[startIndex];
            }
            if (isReverse == false) {
                return array[(startIndex+i) % capacity];
            }
            else {
                if (i <= endIndex) {
                    return array[(endIndex - i) % capacity];
                }
                else {
                    return array[((size-1-i)+startIndex) % capacity];
                }
            }
        }
        void AddEnd(elmtype v) {
            if (isReverse == true) {
                isReverse = false;
                AddFront(v);
                isReverse = true;
                return;
            }
            if(size == capacity) {
                elmtype* tempArr = new elmtype[capacity * 2];
                for(int i = 0; i < size; i++) {
                    tempArr[i] = array[(startIndex+i) % capacity];
                }
                delete [] array;
                array = tempArr;
                capacity*=2;
                startIndex = 0;
                endIndex = size-1;
            }
            if (endIndex+1 == capacity) {
                array[0] = v;
                endIndex = startIndex-1;
            }
            else{ 
                array[endIndex+1] = v;
                endIndex++;
            }
            
            size++;
        }
        void AddFront(elmtype v) {
            if (isReverse == true) {
                isReverse = false;
                AddEnd(v);
                isReverse = true;
                return;
            } 
            if(size == capacity) {
                elmtype* tempArr = new elmtype[capacity * 2];
                for(int i = 0; i < size; i++) {
                    tempArr[i] = array[(startIndex+i) % capacity];
                }
                delete [] array;
                array = tempArr;
                capacity*=2;
                startIndex = 0;
                endIndex = size-1;
            }
            if (startIndex-1 >= 0) {
                array[startIndex-1] = v;
                startIndex--;
            }
            else {
                array[capacity-1] = v;
                startIndex = capacity-1;
            }
            size++;
        }
        void DelEnd() {
            if (isReverse == true) {
                isReverse = false;
                DelFront();
                isReverse = true;
                return;
            }
            if(endIndex - 1 > 0) {
                endIndex = endIndex - 1;
            }
            else {
                endIndex = size - 2;
            }
            size--;
            if (size == capacity/4){
                elmtype* tempArr = new elmtype[capacity / 2];
                for(int i = 0; i < size; i++) {
                    tempArr[i] = array[(startIndex+i) % capacity];
                }
                delete [] array;
                array = tempArr;
            }
        }
        void DelFront() {
            if (isReverse == true) {
                isReverse = false;
                DelEnd();
                isReverse = true;
                return;
            }
            if (startIndex + 1 < capacity) {
                startIndex = startIndex + 1;
            }
            else {
                startIndex = 0;
            }
            size--;
            if (size == capacity/4){
                elmtype* tempArr = new elmtype[capacity / 2];
                for(int i = 0; i < size; i++) {
                    tempArr[i] = array[(startIndex+i) % capacity];
                }
                array = tempArr;
                delete [] tempArr;
            }
        }
        int Length() {
            return size;
        }
        int Capacity() {
            return capacity;
        }
        void Clear() {
            elmtype* tempArray = new elmtype[4];
            delete [] array;

            size = 0;
            capacity = 4;
            array = tempArray;
        }
        void Reverse() {
            if (isReverse == true) {
                isReverse = false;
                
            }
            else {
                isReverse = true;
                
            }
        }
        elmtype recurSelect(CDA<elmtype> arr, int k) {
            int r = rand() % (arr.Length());
            elmtype pivot = arr[r];
            CDA<elmtype> L, E, G;
            for (int x = 0; x < arr.Length(); x++) {
                if (arr[x] < pivot) {
                    L.AddEnd(arr[x]);
                }
                else if (arr[x] == pivot) {
                    E.AddEnd(arr[x]);
                }
                else {
                    G.AddEnd(arr[x]);
                }
            }
            if (k <= L.Length()) {
                return recurSelect(L, k);
            }
            else if (k <= L.Length() + E.Length()) {
                return pivot;
            }
            else {
                return recurSelect(G, k - L.Length() - E.Length());
            }
        }
        elmtype Select(int k) {
            CDA<elmtype> temp = *this;
            return recurSelect(temp, k);
        }
        int partition(CDA<elmtype> &arr, int left, int right) {
            
            int r = left + rand() % (right - left);
            elmtype tempRand = arr[right];
            arr[right] = arr[r];
            arr[r] = tempRand;

            elmtype pivot = arr[right];
            int i = (left - 1);

            for (int j = left; j <= right - 1; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    elmtype temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
            elmtype temp = arr[i+1];
            arr[i+1] = arr[right];
            arr[right] = temp;
            return (i + 1); 
        }
        void quickSort(CDA<elmtype> &arr, int left, int right) {
            if (left < right) {
                int index = partition(arr, left, right);
                quickSort(arr, left, index-1);
                quickSort(arr, index+1, right);
            }
        }

        void Sort() {
            isReverse = false;
            CDA<elmtype> temp = *this;
            quickSort(temp, 0, size-1);
            *this = temp;
            startIndex = 0;
            endIndex = size-1;
        }
        int Search(elmtype e) {
            CDA<elmtype> temp = *this;
            for (int i = 0; i < size; i++) {
                if (temp[i] == e) {
                    return i;
                }
            }
            return -1;
        }
        int BinSearchRecur(CDA<elmtype> &arr, int left, int right, elmtype e) {
            
            int mid = (right + left) / 2;
            if (right >= left) {

                if (arr[mid] == e) {
                    return mid;
                }
                if (arr[mid] > e) {
                    return BinSearchRecur(arr, left, mid-1, e);
                }
                if (arr[mid] < e) {
                    return BinSearchRecur(arr, mid+1, right, e);
                }
            }
            if (mid+1 < size) {
                return ~(mid+1);
            }
            else {
                return ~size;
            }
        }
        int BinSearch(elmtype e) {
            return BinSearchRecur(*this, 0, size-1, e);
        }
        int Start() {
            return startIndex;
        }
        int End() { 
            return endIndex;
        }


};