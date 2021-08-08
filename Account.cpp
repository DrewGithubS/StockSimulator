#include <cstdlib>
#include <iostream>
#include <string.h>

#include "Account.h"
#include "CircularBuffer.h"
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
			// AI->inputs = (int *) malloc(historyLength * 2 * sizeof(int));
			memcpy(AI->inputs, market->history->array, market->historyLength * sizeof(PriceData));
			AI->feedForward();
			float * output = (float *) malloc(7 * sizeof(float));
			output = AI->output;

			// Parsing the AI output
			float maxOfFive = output[0];
			int maxOfFiveIndex = 0;
			for(int i = 1; i < 5; i++) {
				maxOfFiveIndex = output[i] > maxOfFive ? i : maxOfFiveIndex;
				maxOfFive = output[i] > maxOfFive ? output[i] : maxOfFive;
			}

			if(maxOfFiveIndex == 4) {
				int orderCancel = (int) (output[5] * maxOrders);
				market->cancelOrder(orders[orderCancel]);
			} else {
				switch(maxOfFiveIndex) {
					case 0:
						marketBuy((int) (output[5] * 100));
						break;
					case 1:
						marketSell((int) (output[5] * 100));
						break;
					case 2:
						limitBuy((int) (output[5] * 100), (int) (output[6] * 100));
						break;
					case 3:
						limitSell((int) (output[5] * 100), (int) (output[6] * 100));
						break;
					default:
						break;
				}
			}
		}
	}
}

void Account::marketBuy(int sharesIn) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex] = new Order(sharesIn, 0, &money, &shares, true, true, false, this);
	market->handleMarketBuy(orders[currentOrderIndex]);
	currentOrderIndex = (currentOrderIndex+1) < maxOrders ? (currentOrderIndex+1) : 0;
}

void Account::marketSell(int sharesIn) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex] = new Order(sharesIn, 0, &money, &shares, true, false, false, this);
	market->handleMarketSell(orders[currentOrderIndex]);
	currentOrderIndex = (currentOrderIndex+1) < maxOrders ? (currentOrderIndex+1) : 0;
}

void Account::limitBuy(int sharesIn, int price) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex] = new Order(sharesIn, price, &money, &shares, false, true, false, this);
	market->handleLimitBuy(orders[currentOrderIndex]);
	currentOrderIndex = (currentOrderIndex+1) < maxOrders ? (currentOrderIndex+1) : 0;
}

void Account::limitSell(int sharesIn, int price) {
	if(orders[currentOrderIndex]) {
		market->cancelOrder(orders[currentOrderIndex]);
	}
	orders[currentOrderIndex] = new Order(sharesIn, price, &money, &shares, false, false, false, this);
	market->handleLimitSell(orders[currentOrderIndex]);
	currentOrderIndex = (currentOrderIndex+1) < maxOrders ? (currentOrderIndex+1) : 0;
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