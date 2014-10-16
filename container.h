#ifndef _CONTAINTER_H
#define _CONTAINTER_H

#include "utils.h"

#define pa(x) (((x)-1)/2)
#define lchild(x) ((x)*2+1)
#define rchild(x) ((x)*2+2)
#define MAX_HEAP 1
#define MIN_HEAP -1

template <class T>
class heap {
private:
	int max_size;
	int heap_size;
	int inc;
	int mode;
	T* data;
	void init(int init_size, int heap_mode) {
		max_size = init_size;
		mode = heap_mode;
		inc = 10;
		data = new T[max_size];
	}
	void expand() {
		max_size += inc;
		data = (T*) realloc(data, sizeof(T)*max_size);
	}
	void modify(int pos, bool flag = true) {
		T extrema;
		// modify up
		while (flag && pos != 0) {
			if (data[pos]*mode > data[pa(pos)]*mode) {
				extrema = data[pos];
				data[pos] = data[pa(pos)];
				data[pa(pos)] = extrema;
				pos = pa(pos);
			} else {
				break;
			}
		}
		// modify down
		while (pos < heap_size) {
			extrema = data[pos];	
			if (lchild(pos) < heap_size && extrema*mode < data[lchild(pos)]*mode) {
				extrema = data[lchild(pos)];
			}
			if (rchild(pos) < heap_size && extrema*mode < data[rchild(pos)]*mode) {
				extrema = data[rchild(pos)];
			}

			if (lchild(pos) < heap_size && extrema == data[lchild(pos)]) {
				data[lchild(pos)] = data[pos];
				data[pos] = extrema;
				pos = lchild(pos);
			} else if (rchild(pos) < heap_size && extrema == data[rchild(pos)]) {
				data[rchild(pos)] = data[pos];
				data[pos] = extrema;
				pos = rchild(pos);
			} else {
				break;
			}
		}
	}
	void make_heap() {
		T extrema;
		for (int i = pa(heap_size-1); i >= 0; i--) {
			modify(i, false);
		}
	}
public:
	heap(T* start, T* end, int heap_mode) {
		init(end-start, heap_mode);	
		memcpy(data, start, sizeof(T)*(end-start));
		heap_size = end-start;
		make_heap();
	}
	heap(int heap_mode) {
		init(10, heap_mode);	
		heap_size = 0;
	}
	~heap() {
		delete[] data;
		data = NULL;
	}
	T extract() {
		T ret;
		if (heap_size <= 0)
			throw "can not extract from an empty heap";
		
		ret = data[0];
		data[0] = data[--heap_size];	
		modify(0);
		return ret;
	}
	bool is_empty() {
		return heap_size == 0;
	}
	void push(T val) {
		if (heap_size+1 > max_size) {
			expand();
		}
		data[heap_size] = val;
		modify(heap_size);
		heap_size++;
	}
	void print_heap() {
		print_vec(data, heap_size, "heap data");
	}
	int size() {
		return heap_size;		
	}
};

#endif
