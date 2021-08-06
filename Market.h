#ifndef MARKET_H
#define MARKET_H

class Order;

template<class Type>
class SortedList;

class Market {
public:
	int volume;
	int lastPrice;
	SortedList<Order> * buyers;
	SortedList<Order> * sellers;

	Market(int lastPrice);

	void handleMarketBuy(Order * order);
	void handleMarketSell(Order * order);
	void handleLimitBuy(Order * order);
	void handleLimitSell(Order * order);

	void cancelOrder(Order * order);
};

#endif