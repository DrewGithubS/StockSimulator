#ifndef MARKET_H
#define MARKET_H

class Order;

template<class Type>
class SortedList;

template<class Type>
class CircularBuffer;

struct PriceData {
	int price;
	int volume;
}

class Market {
public:
	int volume;
	int lastPrice;
	SortedList<Order> * buyers;
	SortedList<Order> * sellers;
	int historyLength;
	CircularBuffer<int> history;


	Market(int lastPrice, int historyLengthIn);

	void handleMarketBuy(Order * order);
	void handleMarketSell(Order * order);
	void handleLimitBuy(Order * order);
	void handleLimitSell(Order * order);

	void cancelOrder(Order * order);
	void updatePrice();
	void print();
};

#endif