#ifndef _MyHeap_H_
#define _MyHeap_H_

#include"stdafx.h"

template <class Obj>
class MyHeap{
public:
	typedef function<int(const Obj* a, const Obj* b)> compareCallback;
public:
	MyHeap(){}
	MyHeap(const int& size, compareCallback compare) :
		_compare(nullptr)
	{
		heap = new Obj*[size + 1];
		_size = size + 1;
		_count = 0;
		_compare = compare;
	}

	Obj* peek(){ return heap[1]; }

	int maxSize(){ return _size; }

	int size(){ return _count; }

	bool put(Obj* obj){
		if (_count + 1 < _size){
			heap[++_count] = obj;

			int i = _count;
			int parent = i >> 2; // = i / 2
			Obj* tmp = heap[i];
			Obj* v;

			if (_compare != nullptr){
				while (parent > 0){
					v = heap[parent];
					if (_compare(tmp, v) > 0){
						heap[i] = v;
						i = parent;
						parent >>= 1;
					}
					else
						break;
				}
			}
			else{
				while (parent > 0){
					v = heap[parent];
					if (tmp - v > 0){
						heap[i] = v;
						i = parent;
						parent >>= 1;
					}
					else
						break;
				}	
			}
			heap[i] = tmp;
			return true;
		}
		return false;
	}

	Obj* pop(){
		if (_count >= 1){
			Obj* o = heap[1];
			heap[1] = heap[_count];
			//delete heap[_count];

			int i = 1;
			int child = 1 << 1;
			Obj* tmp = heap[i];
			Obj* v;
			if (_compare != nullptr){
				while (child < _count){
					if (child < _count - 1){
						if (_compare(heap[child], heap[int(child + 1)]) < 0)
							child++;
					}
					v = heap[child];
					if (_compare(tmp, v) < 0){
						heap[i] = v;
						i = child;
						child <<= 1;
					}
					else
						break;
				}
			}
			else{
				while (child < _count)
				{
					if (child < _count - 1)
					{
						if (heap[child] - heap[int(child + 1)] < 0)
							child++;
					}
					v = heap[child];
					if (tmp - v < 0)
					{
						heap[i] = v;
						i = child;
						child <<= 1;
					}
					else break;
				}
			}
			heap[i] = tmp;
			_count--;
			return o;
		}
		return new Obj();
	}

	void clear(){
		delete heap;
		heap = new Obj*[_size];
		_count = 0;
	}

public:
	Obj** heap;
private:
	int _size;
	int _count;
	compareCallback _compare;
};


#endif _MyHeap_H_