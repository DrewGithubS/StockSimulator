#include <iostream>

#ifndef SORTEDLIST_H
#define SORTEDLIST_H

template<class Type>
class SortedList {
public:
	Type * data;
	SortedList * next;
	bool isSet;
	bool highToLowSorting;

	SortedList<Type>(Type * dataIn, SortedList * nextIn, bool isSetIn, bool highToLowSortingIn) {
		data = dataIn;
		next = nextIn;
		isSet = isSetIn;
		highToLowSorting = highToLowSortingIn;
	}

	SortedList<Type> * insert(Type * dataIn) {
		if(!isSet) {
			data = dataIn;
			isSet = true;
			return this;
		}
		if(((*dataIn) < (*(data))) ^ (highToLowSorting)) {
			return new SortedList(dataIn, this, true, highToLowSorting);
		} else {
			SortedList<Type> * temp = this;
			while(true) {
				if(!temp->isSet) {
					temp->data = dataIn;
					temp->isSet = true;
					return this;
				}
				if(temp->next == 0) {
					temp->next = new SortedList(dataIn, 0, true, highToLowSorting);
					return this;
				}
				if(((*dataIn) < (*(temp->next->data))) ^ highToLowSorting) {
					temp->next = new SortedList(dataIn, temp->next, true, highToLowSorting);
					return this;
				}
				temp = temp->next;
			}
		}
	}

	SortedList<Type> * remove(Type * dataIn) {
		if(data == dataIn) {
			if(next == 0) {
				isSet = false;
				return this;
			}
			SortedList * output = next;
			next->deleteOldHead(this);
			return output;
		}
		if(next == 0 || !next->isSet) {
			return this;
		}
		SortedList<Type> * temp = this;
		while(temp != 0 && temp->next != 0 && temp->next->isSet) {
			if(temp->next->data == dataIn) {
				SortedList<Type> * toDelete = temp->next;
				temp->next = temp->next->next;
				delete toDelete;
			}
			temp = temp->next;
		}
		return this;
	}

	SortedList<Type> * deleteCurrent() {
		if(next == 0) {
			isSet = false;
			return this;
		}
		SortedList<Type> * output = next;
		next->deleteOldHead(this);
		return output;
	}

	void deleteOldHead(SortedList * toBeDeleted) {
		delete toBeDeleted;
	}

	void print()  {
		if(!isSet) {
			return;
		} else {
			SortedList<Type> * temp = this;
			while(temp != 0 && temp->isSet) {
				temp->data->print();
				temp = temp->next;
			}
		}
	}

	void printAdded(bool buying) {
		SortedList<Type> * temp = this;
		while(temp != 0 && temp->isSet) {
			int strikePrice = temp->data->price;
			int sharesAtStrike = temp->data->fillsLeft;
			while((temp = temp->next) != 0 && 
			  temp->isSet && 
		      temp->data->price == strikePrice) {
				sharesAtStrike += temp->data->fillsLeft;
			}
			std::cout << sharesAtStrike << " " << (buying ? "B" : "S") << " @ " << strikePrice/100 << "." << strikePrice%100 << std::endl;
		}
	}
};

#endif