#include <cstdlib>

#include "Account.h"
#include "Order.h"
#include "Market.h"

Account::Account(int startingMoney, int startingShares, int maxOrdersIn, Market * marketIn) {
	money = startingMoney;
	shares = startingShares;
	maxOrders = maxOrdersIn;
	orders = (Order **) calloc(maxOrders, sizeof(Order *));
	market = marketIn;
	currentOrderIndex = 0;
}

void Account::marketBuy(int shares) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex++] = new Order(shares, 0, &money, &shares, true, true, false, this);
	market->handleMarketBuy(orders[currentOrderIndex]);
}

void Account::marketSell(int shares) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex++] = new Order(shares, 0, &money, &shares, true, false, false, this);
	market->handleMarketSell(orders[currentOrderIndex]);
}

void Account::limitBuy(int shares, int price) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex++] = new Order(shares, price, &money, &shares, false, true, false, this);
	market->handleLimitBuy(orders[currentOrderIndex]);
}

void Account::limitSell(int shares, int price) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex++] = new Order(shares, price, &money, &shares, false, false, false, this);
	market->handleLimitSell(orders[currentOrderIndex]);
}

void Account::cancelOrder(Order * order) {
	market->cancelOrder(order);
	for(int i = 0; i < maxOrders; i++) {
		if(orders[i] == order) {
			orders[i] = 0;
			break;
		}
	}
}