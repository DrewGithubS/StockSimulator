#ifndef ACCOUNT_H
#define ACCOUNT_H

class Order;
class Market;

class Account {
public:
	int money;
	int shares;
	int currentOrderIndex;
	int maxOrders;
	Order ** orders;
	Market * market;
	bool isActive;

	Account(int startingMoney, int startingShares, int maxOrdersIn, Market * marketIn);
	
	void marginCall();
	void doAction();
	void marketBuy(int shares);
	void marketSell(int shares);
	void limitBuy(int shares, int price);
	void limitSell(int shares, int price);
	void cancelOrder(Order * order);
};

#endif