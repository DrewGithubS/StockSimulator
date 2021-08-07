#include <string.h>
#include <stdlib.h>

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

template<class Type>
class CircularBuffer {
public:
	int size;
	Type * array;
	int index;

	CircularBuffer(int sizeIn) {
		size = sizeIn;
		array = (Type *) malloc(sizeIn * sizeof(Type));
		index = 0;
	}

	void append(Type in) {
		if(index < size) {
			array[index++] = in;
		} else {
			memmove(&array[0], &array[1], (size) * sizeof(Type));
			array[size-1] = in;
		}
	}
};

#endif