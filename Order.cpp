#include <iostream>

#include "Order.h"

Order::Order(int sharesIn, int priceIn, int * orderersMoneyIn, int * orderersSharesIn, bool isMarketOrderIn, bool isBuyingIn, bool isLiquidationIn, const Account * senderIn) {
	fillsLeft = sharesIn;
	price = priceIn;
	orderersMoney = orderersMoneyIn;
	orderersShares = orderersSharesIn;
	isMarketOrder = isMarketOrderIn;
	isBuying = isBuyingIn;
	isLiquidation = isLiquidationIn;
	sender = senderIn;
}

bool Order::operator==(const Order & order1) {
	return order1.price == price;
}

bool Order::operator<(const Order & order1) {
	return order1.price < price;
}

bool Order::operator>(const Order & order1) {
	return order1.price > price;
}



void Order::print() {
	std::cout << fillsLeft << " " << (isBuying ? "B" : "S") << " @ " << price << std::endl;
}