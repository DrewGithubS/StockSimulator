#ifndef ACCOUNTLIST_H
#define ACCOUNTLIST_H

class Account;
class Market;

class AccountList {
	public:
		Account ** accounts;
		int accountsCreated;

		AccountList(int accountsToCreate, int startingCash, int startingShares, int maxOrders, Market * marketPointer);
		void doActions(int amount);
};

#endif