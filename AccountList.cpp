#include <stdlib.h>

#include "AccountList.h"
#include "Account.h"
#include "Market.h"

		
AccountList::AccountList(int accountsToCreate, int startingCash, int startingShares, int maxOrders, Market * marketPointer) {
	accountsCreated = accountsToCreate;
	accounts = (Account **) malloc(accountsToCreate * sizeof(Account));
	for(int i = 0; i < accountsToCreate; i++) {
		accounts[i] = new Account(startingCash, startingShares, maxOrders, marketPointer);
	}
}

void AccountList::doActions(int amount) {
	for(int i = 0; i < amount; i++) {
		for(int j = 0; j < accountsCreated; j++) {
			accounts[j]->doAction();
		}
	}
}