#include <iostream>

#include "Market.h"
#include "AccountList.h"
#include "Account.h"
#include "Order.h"

// Values are in pennies
int STARTINGCASH = 10000000;
int STARTINGSTOCKPRICE = 10000;
int ACCOUNTSTOCREATE = 2;
int STARTINGSHARES = 1000;
int MAXORDERSPERUSER = 10;
int PENNIESINADOLLAR = 100;

int main() {
	std::cout << "1" << std::endl;
	Market * market = new Market(STARTINGSTOCKPRICE);
	std::cout << "2" << std::endl;
	AccountList * accountList = new AccountList(ACCOUNTSTOCREATE, STARTINGCASH, STARTINGSHARES, MAXORDERSPERUSER, market);
	std::cout << "3" << std::endl;
	Order * temp;
	std::cout << "4" << std::endl;
	for(int j = 0; j < MAXORDERSPERUSER/2; j++) {
		std::cout << " " << j << std::endl;
		for(int i = 0; i < accountList->accountsCreated; i++) {
			std::cout << "  " << i << std::endl;
			accountList->accounts[i]->limitBuy(1, STARTINGSTOCKPRICE - (j * PENNIESINADOLLAR));
			accountList->accounts[i]->limitSell(1, STARTINGSTOCKPRICE + (j * PENNIESINADOLLAR));
		}
	}
	std::cout << "4" << std::endl;

	accountList->accounts[0]->limitBuy(2, 10100);
	std::cout << "5" << std::endl;

	market->print();
	std::cout << "6" << std::endl;
}