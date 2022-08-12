#include "heads.h"
#include "card.h"
#include "computer_player.h"
#include <iostream>

Heads::Heads() {
	Head::resetHeadCount();
}

void Heads::createNewHead(std::shared_ptr<Card> card)
{
	auto newHead = std::make_shared<Head>(card);
	headCollection.push_back(newHead);
}

bool Heads::checkHeadNumber(int n) const
{
	if (n < headCollection.at(0)->getIndex() || n > headCollection.at(headCollection.size() - 1)->getIndex()) return false;
	return true;
}

int Heads::checkCutHead(int headIndex, std::shared_ptr<Card> card) const
{
	if (headIndex != headCollection[0]->getIndex() || card->getSuit() == 'J') return 1;
	for (int i = 0; i < headCollection.size(); i++) {
		// Deep-copy each head and try if we can add the card to it. 
		auto headCopy = std::make_unique<Head>(*(headCollection[i]));
		if (headCopy->addCard(card) != 0) return headCollection[i]->getIndex();
	}
	return 0;
}

// return value: 1 - card added;
//				 2 - card added and no further draws permitted; 
//				 3 - Ace on the top and the card is added on it
//				 0 - cannot add the card to the head [card not added]
//				 4 - need to cut a head.
int Heads::addCardToHead(int headIndex, std::shared_ptr<Card> card)
{
	if (!checkHeadNumber(headIndex)) {
		throw BadHeadIndexException();
	}
	int addResult = headCollection[headIndex - headCollection[0]->getIndex()]->addCard(card);
	if (addResult == 0 && checkCutHead(headIndex, card) == 0) {
		return 4;
	}
	return addResult;
}

std::shared_ptr<Head> Heads::popHead()
{
	auto head = headCollection[0];
	headCollection.erase(headCollection.begin());
	return head;
}

int Heads::determineFirstValidHeadIndex(std::shared_ptr<Card> card) const
{
	int checkRes = checkCutHead(headCollection[0]->getIndex(), card);
	if (checkRes == 0) {
		return headCollection[0]->getIndex();
	}
	return checkRes;
}

int Heads::getSize() const
{
	return headCollection.size();
}

std::pair<int, int> Heads::getHeadIndexRange() const
{
	return std::make_pair(headCollection[0]->getIndex(), headCollection[headCollection.size() - 1]->getIndex());
}

std::ostream& operator<<(std::ostream& os, const Heads& heads)
{
	os << "Heads:" << std::endl;
	for (auto head : heads.headCollection) {
		os << *head << std::endl;
	}
	return os;
}
