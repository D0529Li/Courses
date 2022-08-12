#include <cstdlib>
#include <vector>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

const int initialSize = 1;

enum TypeOfHeap {
	MaxHeap, MinHeap, Unknown
};

class Heap {
public:
	Heap(TypeOfHeap newType = Unknown) {
		data = nullptr;
		size = 0;
		capacity = 0;
		type = newType;
	}
	int getSize() {
		return size;
	}
	int getRoot() {
		if (size < 1) {
			throw exception();
		}
		return data[0];
	}
	void insert(int e) {
		if (size == 0 && capacity == 0 && data == nullptr) {
			capacity = initialSize;
			data = new int[capacity];
		}
		else {
			if (size == capacity) {
				capacity = capacity * 2 + 1;
				int* newData = new int[capacity];
				for (int j = 0; j < size; j++) {
					newData[j] = data[j];
				}
				delete[] data;
				data = newData;
			}
		}
		data[size] = e;
		fix_up(size++);
	}
	void swapWithRoot(int e) {
		if (capacity < 1) {
			throw exception();
		}
		data[0] = e;
		fix_down(0);
	}
	int compareWithRoot(int x) { // -1: x < root; 0: x == root; 1: x > root;
		if (size < 1) {
			throw exception();
		}
		if (x < data[0]) return -1;
		else if (x == data[0]) return 0;
		return 1;
	}
protected:
	void fix_up(int i) {
		while (i > 0) {
			int parentIndex = (i - 1) / 2;
			if (type == MaxHeap) {
				if (data[parentIndex] < data[i]) {
					swap(data[parentIndex], data[i]);
				}
				else break;
			}
			else if (type == MinHeap) {
				if (data[parentIndex] > data[i]) {
					swap(data[parentIndex], data[i]);
				}
				else break;
			}
			i = parentIndex;
			parentIndex = (i - 1) / 2;
		}
	}
	void fix_down(int i) {
		while (i < capacity / 2) {
			int childIndex = 2 * i + 1;
			if (type == MaxHeap) {
				if (data[childIndex + 1] > data[childIndex]) {
					childIndex++;
				}
				if (data[childIndex] > data[i]) {
					swap(data[childIndex], data[i]);
					i = childIndex;
				}
				else break;
			}
			else if (type == MinHeap) {
				if (childIndex + 1 < size && data[childIndex + 1] < data[childIndex]) {
					childIndex++;
				}
				if (data[childIndex] < data[i]) {
					swap(data[childIndex], data[i]);
					i = childIndex;
				}
				else break;
			}
		}
	}
private:
	int* data;
	int size;
	int capacity;
	TypeOfHeap type;
};

class OnlineMedian {
public:
	OnlineMedian() {
		H_hi = new Heap(TypeOfHeap::MinHeap);
		H_lo = new Heap(TypeOfHeap::MaxHeap);
	}

	void insert(int x) {
		if (H_hi->getSize() > H_lo->getSize()) {
			if (H_hi->compareWithRoot(x) == -1) {
				H_lo->insert(x);
			}
			else {
				H_lo->insert(H_hi->getRoot());
				H_hi->swapWithRoot(x);
			}
		}
		else if (H_hi->getSize() == H_lo->getSize()) {
			if (H_hi->getSize() == 0) {
				H_hi->insert(x);
			}
			else if (H_lo->compareWithRoot(x) == 1) {
				H_hi->insert(x);
			}
			else {
				H_hi->insert(H_lo->getRoot());
				H_lo->swapWithRoot(x);
			}
		}
	}

	int getMedian() {
		return H_hi->getRoot();
	}

private:
	Heap* H_hi;
	Heap* H_lo;
};


//-----------------------------------------//
// reads integers, prints the median       //
//-----------------------------------------//
int main() {
	string line;
	OnlineMedian med = OnlineMedian();

	while (true) {
		if (!getline(cin, line))
			break;
		int a = atoi(line.c_str());
		med.insert(a);
		cout << "reading: " << a << endl;
		cout << "median so far: " << med.getMedian() << endl;
	};
}


