#include <iostream>

#include "SortedList.h"

class IntegerTest {
public:
	int value;
	int creationNumber;
	IntegerTest(int valueIn, int creationNumberIn) {
		value = valueIn;
		creationNumber = creationNumberIn;
	}

	void print() {
		std::cout << value << " " << creationNumber << std::endl;
	}
};

bool operator==(const IntegerTest & in1, const IntegerTest & in2) {
	return in1.value == in2.value;
}

bool operator<(const IntegerTest & in1, const IntegerTest & in2) {
	return in1.value < in2.value;
}

bool operator>(const IntegerTest & in1, const IntegerTest & in2) {
	return in1.value > in2.value;
}

int main() {
	SortedList<IntegerTest> * list = new SortedList<IntegerTest>(0, 0, false, false);
	IntegerTest * test1 = new IntegerTest(3, 0);
	IntegerTest * test2 = new IntegerTest(5, 1);
	IntegerTest * test3 = new IntegerTest(2, 2);
	IntegerTest * test4 = new IntegerTest(3, 3);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test1);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test2);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test3);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test4);
	list->print();
	std::cout << "\nDeleting one..." << std::endl;
	list = list->deleteCurrent();
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test3);
	list->print();
	std::cout << "\nDeleting one..." << std::endl;
	list = list->remove(test4);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	IntegerTest * test5 = new IntegerTest(7, 4);
	list = list->insert(test5);
	list->print();
	std::cout << "\nDeleting four..." << std::endl;
	list = list->deleteCurrent();
	list->print();
	list = list->deleteCurrent();
	list->print();
	list = list->deleteCurrent();
	list->print();
	list = list->deleteCurrent();
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test1);
	list->print();
	std::cout << "\nDeleting one..." << std::endl;
	list = list->remove(test1);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	list = list->insert(test1);
	list->print();
	std::cout << "\nDeleting one..." << std::endl;
	list = list->remove(test5);
	list->print();
	std::cout << "\nInserting next..." << std::endl;
	IntegerTest * test6 = new IntegerTest(-1, 4);
	list = list->insert(test6);
	list->print();
}