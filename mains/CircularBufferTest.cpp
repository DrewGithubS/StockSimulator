#include <iostream>
#include "CircularBuffer.h"

void printArray(int * array, int size) {
	for(int i = 0; i < size; i++) {
		std::cout << array[i] << " ";
	}
	std::cout << std::endl;
}

int main() {
	CircularBuffer<int> test = CircularBuffer<int>(10);
	for(int i = 0; i < 50; i++) {
		test.append(i);
		std::cout << i << std::endl;
		printArray(test.array, 10);
	}
}