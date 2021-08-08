#include "Account.h"
#include "Market.h"
#include "Order.h"
#include "SortedList.h"

Market::Market(int lastPriceIn, int historyLengthIn) {
	lastPrice = lastPriceIn;
	buyers = new SortedList<Order>(0, 0, false, true);
	sellers = new SortedList<Order>(0, 0, false, false);
	historyLength = historyLengthIn;
	history = new CircularBuffer<int>(historyLength);
}

void Market::handleMarketBuy(Order * buyOrder) {
	Order * sellOrder;
	while(buyOrder->fillsLeft > 0 && sellers->isSet) {
		sellOrder = sellers->data;
		// Buy as much as possible without overspending or over using the next sell order.
		int fillAmount = (buyOrder->fillsLeft) < (sellOrder->fillsLeft) ? (buyOrder->fillsLeft) : sellOrder->fillsLeft;
		if(!sellOrder->isLiquidation) {
			fillAmount = ((*(buyOrder->orderersMoney)) / (sellOrder->price)) < fillAmount ? ((*(buyOrder->orderersMoney)) / (sellOrder->price)) : fillAmount;
		}

		// Take the money from the buyer
		*(buyOrder->orderersMoney) -= fillAmount * sellOrder->price;
		// Give the money to the seller
		*(sellOrder->orderersMoney) += fillAmount * sellOrder->price;
		// Give the shares to the buyer
		*(buyOrder->orderersShares) += fillAmount;
		// Take the shares from the orders fillsLeft
		buyOrder->fillsLeft -= fillAmount;
		sellOrder->fillsLeft-= fillAmount;

		lastPrice = sellOrder->price;
		volume += fillAmount;

		if(sellOrder->fillsLeft == 0) {
			sellOrder->sender->orders[sellOrder->sender->currentOrderIndex] = 0;
			delete sellOrder;
			sellers = sellers->deleteCurrent();
		}
	}
	buyOrder->sender->orders[buyOrder->sender->currentOrderIndex] = 0;
	delete buyOrder;
}

void Market::handleMarketSell(Order * sellOrder) {
	Order * buyOrder;
	while(buyOrder->fillsLeft > 0 && buyers->isSet) {
		buyOrder = sellers->data;
		// Sell as much as possible without overspending or over using the next buy order.
		int fillAmount = (sellOrder->fillsLeft) < (buyOrder->fillsLeft) ? (sellOrder->fillsLeft) : buyOrder->fillsLeft;
		if(!sellOrder->isLiquidation) {
			int margin = ((*(sellOrder->orderersMoney)) + (*(sellOrder->orderersShares) * lastPrice * 2));
			fillAmount = (margin / (buyOrder->price)) < fillAmount ? (margin / (buyOrder->price)) : fillAmount;
		}

		// Take the shares from the seller
		*(buyOrder->orderersShares) -= fillAmount;
		// Give the money to the seller
		*(sellOrder->orderersMoney) += fillAmount * buyOrder->price;
		// Take the shares from the orders fillsLeft
		sellOrder->fillsLeft -= fillAmount;
		buyOrder->fillsLeft-= fillAmount;

		lastPrice = buyOrder->price;
		volume += fillAmount;
		if(buyOrder->fillsLeft == 0) {
			delete buyOrder;
			buyOrder->sender->orders[buyOrder->sender->currentOrderIndex] = 0;
			buyers = buyers->deleteCurrent();
		}
	}
	sellOrder->sender->orders[sellOrder->sender->currentOrderIndex] = 0;
	delete sellOrder;
}

void Market::handleLimitBuy(Order * buyOrder) {
	if(sellers->isSet) {
		Order * sellOrder = sellers->data;
		while(sellers != 0 && buyOrder->fillsLeft > 0 && sellers->isSet && ((*sellOrder) <= (*buyOrder))) {
			sellOrder = sellers->data;
			// Buy as much as possible without overspending or over using the next sell order.
			int fillAmount = (buyOrder->fillsLeft) < (sellOrder->fillsLeft) ? (buyOrder->fillsLeft) : sellOrder->fillsLeft;
			if(!sellOrder->isLiquidation) {
				fillAmount = ((*(buyOrder->orderersMoney)) / (sellOrder->price)) < fillAmount ? ((*(buyOrder->orderersMoney)) / (sellOrder->price)) : fillAmount;
			}
			// Take the money from the buyer
			*(buyOrder->orderersMoney) -= fillAmount * sellOrder->price;
			// Give the money to the seller
			*(sellOrder->orderersMoney) += fillAmount * sellOrder->price;
			// Give the shares to the buyer
			*(buyOrder->orderersShares) += fillAmount;
			// Take the shares from the orders fillsLeft
			buyOrder->fillsLeft -= fillAmount;
			sellOrder->fillsLeft-= fillAmount;

			lastPrice = sellOrder->price;
			volume += fillAmount;
			if(sellOrder->fillsLeft == 0) {
				sellOrder->sender->orders[sellOrder->sender->currentOrderIndex] = 0;
				delete sellOrder;
				sellers = sellers->deleteCurrent();
				sellOrder = sellers->data;
			}
		}
		if(buyOrder->fillsLeft > 0) {
			// Make sure the buyer isn't over-spending
			buyOrder->fillsLeft = (*(buyOrder->orderersMoney) / buyOrder->price) < buyOrder->fillsLeft ? (*(buyOrder->orderersMoney) / buyOrder->price) : buyOrder->fillsLeft;
			// Take the money from the orderer 
			*(buyOrder->orderersMoney) -= buyOrder->fillsLeft * buyOrder->price;
			buyers = buyers->insert(buyOrder);
		} else {
			buyOrder->sender->orders[buyOrder->sender->currentOrderIndex] = 0;
			delete buyOrder;
		}
	} else {
		// Make sure the buyer isn't over-spending
		buyOrder->fillsLeft = (*(buyOrder->orderersMoney) / buyOrder->price) < buyOrder->fillsLeft ? (*(buyOrder->orderersMoney) / buyOrder->price) : buyOrder->fillsLeft;
		// Take the money from the orderer 
		*(buyOrder->orderersMoney) -= buyOrder->fillsLeft * buyOrder->price;
		buyers = buyers->insert(buyOrder);
	}
}

void Market::handleLimitSell(Order * sellOrder) {
	if(buyers->isSet) {
		Order * buyOrder = buyers->data;
		std::cout << "MARKER BELOW:" << std::endl;
		std::cout << buyOrder->price << " " << sellOrder->price << std::endl;
		std::cout << (sellOrder->fillsLeft > 0) << " " << (buyers->isSet) << " " << ((*sellOrder) <= (*buyOrder)) << std::endl;
		while(buyOrder != 0 && sellOrder->fillsLeft > 0 && buyers->isSet && ((*sellOrder) <= (*buyOrder))) {
			std::cout << "THERE IS AN ORDER HERE" << std::endl;
			buyOrder = buyers->data;
			// Sell as much as possible without overspending or over using the next buy order.
			std::cout << "TEST3" << std::endl;
			std::cout << (sellOrder->fillsLeft) << std::endl;
			std::cout << buyOrder << std::endl;
			std::cout << (buyOrder->fillsLeft) << std::endl;
			int fillAmount = (sellOrder->fillsLeft) < (buyOrder->fillsLeft) ? (sellOrder->fillsLeft) : buyOrder->fillsLeft;
			std::cout << "TEST2.5" << std::endl;
			if(!sellOrder->isLiquidation) {
				int margin = ((*(sellOrder->orderersMoney)) + (*(sellOrder->orderersShares) * lastPrice * 2));
				fillAmount = (margin / (sellOrder->price)) < fillAmount ? (margin / (sellOrder->price)) : fillAmount;
			}
			std::cout << "TEST2" << std::endl;
			// Take the shares from the seller
			*(buyOrder->orderersShares) -= fillAmount;
			// Give the money to the seller
			*(sellOrder->orderersMoney) += fillAmount * buyOrder->price;
			// Take the shares from the orders fillsLeft
			sellOrder->fillsLeft -= fillAmount;
			buyOrder->fillsLeft-= fillAmount;
			std::cout << "TEST1" << std::endl;
			lastPrice = buyOrder->price;
			volume += fillAmount;
			if(buyOrder->fillsLeft == 0) {
				buyOrder->sender->orders[buyOrder->sender->currentOrderIndex] = 0;
				delete buyOrder;
				buyers = buyers->deleteCurrent();
				buyOrder = buyers->data;
			}
		}
		if(sellOrder->fillsLeft > 0) {
			// Make sure the buyer isn't over-spending
			int margin = ((*(sellOrder->orderersMoney)) + (*(sellOrder->orderersShares) * lastPrice * 2));
			int fillAmount = (margin / (buyOrder->price)) < fillAmount ? (margin / (buyOrder->price)) : fillAmount;
			sellOrder->fillsLeft = (margin / sellOrder->price) < sellOrder->fillsLeft ? (margin / sellOrder->price) : sellOrder->fillsLeft;
			// Take the money from the orderer 
			*(sellOrder->orderersShares) -= sellOrder->fillsLeft;
			std::cout << "MOST RECENT BUYER: " << buyers->data->price << std::endl;
			sellers = sellers->insert(sellOrder);
		} else {
			sellOrder->sender->orders[buyOrder->sender->currentOrderIndex] = 0;
			delete sellOrder;
		}
	} else {
		// Make sure the seller isn't over-spending
		int margin = ((*(sellOrder->orderersMoney)) + (*(sellOrder->orderersShares) * lastPrice * 2));
		int fillAmount = (margin / (sellOrder->price)) < fillAmount ? (margin / (sellOrder->price)) : fillAmount;
		sellOrder->fillsLeft = (margin / sellOrder->price) < sellOrder->fillsLeft ? (margin / sellOrder->price) : sellOrder->fillsLeft;
		// Take the money from the orderer 
		*(sellOrder->orderersShares) -= sellOrder->fillsLeft;
		sellers = sellers->insert(sellOrder);
	}
}

void Market::cancelOrder(Order * order) {
	if(order->isBuying) {
		*(order->orderersMoney) += order->fillsLeft * order->price;
		buyers = buyers->remove(order);
	} else {
		*(order->orderersShares) += order->fillsLeft;
		sellers = sellers->remove(order);
	}
}

void Market::updatePrice() {
	history->append(PriceData{lastPrice, volume});
	volume = 0;
}

void Market::print() {
	std::cout << "Current Price: " << lastPrice/100 << "." << lastPrice%100 << std::endl;
	std::cout << "Current Volume: " << volume << std::endl;
	std::cout << "\nCurrent Order Book:" << std::endl;
	// Printing the order book...
	SortedList<Order> * invertedBuyList = new SortedList<Order>(buyers->isSet ? buyers->data : 0, 0, buyers->isSet, false);
	SortedList<Order> * tempBuy = buyers;
	SortedList<Order> * invertedSellList = new SortedList<Order>(sellers->isSet ? sellers->data : 0, 0, sellers->isSet, true);
	SortedList<Order> * tempSell = sellers;
	if(tempBuy != 0) {
		while((tempBuy = tempBuy->next) != 0 && tempBuy->isSet) {
			invertedBuyList = invertedBuyList->insert(tempBuy->data);
		}
	}
	if(tempSell != 0) {
		while((tempSell = tempSell->next) != 0 && tempSell->isSet) {
			invertedSellList = invertedSellList->insert(tempSell->data);
		}
	}
	invertedSellList->printAdded(false);
	std::cout << std::endl;
	buyers->printAdded(true);
}