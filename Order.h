#ifndef ORDER_H
#define ORDER_H

class Account;

class Order {
public:
	int fillsLeft;
	int price;
	int * orderersMoney;
	int * orderersShares;
	bool isMarketOrder;
	bool isBuying;
	bool isLiquidation;
	const Account * sender;

	Order(int sharesIn, int priceIn, int * orderersMoneyIn, int * orderersSharesIn, bool isMarketOrderIn, bool isBuyingIn, bool isLiquidationIn, const Account * senderIn);

	bool operator==(const Order & order1);
	bool operator<(const Order & order1);
	bool operator>(const Order & order1);

	void print();


};

#endif