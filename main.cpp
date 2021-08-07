#include <iostream>

#include "Market.h"
#include "AccountList.h"
#include "Account.h"
#include "Order.h"
#include "SortedList.h"

// Values are in pennies
int STARTINGCASH = 100000000;
int STARTINGSTOCKPRICE = 10000;
int ACCOUNTSTOCREATE = 2;
int STARTINGSHARES = 1000;
int MAXORDERSPERUSER = 10;
int PENNIESINADOLLAR = 100;

int main() {
	Market * market = new Market(STARTINGSTOCKPRICE);
	AccountList * accountList = new AccountList(ACCOUNTSTOCREATE, STARTINGCASH, STARTINGSHARES, MAXORDERSPERUSER, market);
	Order * temp;
	for(int j = 1; j < MAXORDERSPERUSER/2+1; j++) {
		for(int i = 0; i < accountList->accountsCreated; i++) {
			accountList->accounts[i]->limitBuy(1, STARTINGSTOCKPRICE - (j * PENNIESINADOLLAR));
		}
	}
	market->print();
	for(int j = 1; j < MAXORDERSPERUSER/2+1; j++) {
		std::cout << " " << j << std::endl;
		for(int i = 0; i < accountList->accountsCreated; i++) {
			std::cout << "  " << i << std::endl;
			accountList->accounts[i]->limitSell(1, STARTINGSTOCKPRICE + (j * PENNIESINADOLLAR));
		}
	}

	market->print();

	// accountList->accounts[0]->limitBuy(2, 10100);
	// accountList->accounts[0]->limitBuy(2, 10500);
	accountList->accounts[0]->limitSell(50, 9900);
	accountList->accounts[0]->limitBuy(60, 11000);
	std::cout << "END!" << std::endl;
	// accountList->accounts[0]->limitSell(2, 9900);

	market->print();
}