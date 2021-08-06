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
	isActive = true;
}

void Account::marginCall() {
	// Begin liquidation
	for(int i = 0; i < maxOrders; i++) {
		market->cancelOrder(orders[i]);
	}

	bool buying = (shares < 0) ? true : false;
	int sharesToCover = (shares < 0) ? (-shares) : shares;

	// Make a new market order to cover 
	//					                                Market?  Buy?    Liquidating?
	Order * order = new Order(sharesToCover, 0, &money, &shares, true, buying, true, this);
	if(buying) {
		market->handleMarketBuy(order);
	} else {
		market->handleMarketSell(order);
	}
	isActive = false;
}

void Account::doAction() {
	if(isActive) {
		int totalCash = money;
		int totalShares = shares;
		for(int i = 0; i < maxOrders; i++) {
			// Gets the shares that the user has including orders that haven't been filled.
			totalCash += orders[i]->fillsLeft * orders[i]->price * orders[i]->isBuying;
			totalShares -= orders[i]->fillsLeft * !(orders[i]->isBuying);
		}

		int equity = totalCash + totalShares * market->lastPrice;

		if(equity < 0) {
			marginCall();
		} else {
			// Get action
			// Do action
		}
	}
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