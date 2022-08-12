#pragma once

#include <vector>
#include <memory>

class Card;

class Head
{
	const int index;
	std::vector<std::shared_ptr<Card>> cards;

	friend std::ostream& operator<<(std::ostream& os, const Head& head);
public:
	Head();
	Head(std::shared_ptr<Card> card); // If the card is a joker, then set its value to "2".
	Head(const Head& otherHead); // Copy constructor.
	// return value: 1 - card added;
	//				 2 - card added and no further draws permitted;
	//				 3 - Ace on the top and the card is added on it
	//				 0 - cannot add the card to the head [card not added]
	int addCard(std::shared_ptr<Card> card);
	std::vector<std::shared_ptr<Card>> popAllCards(); // delete all cards in the head and return a vector containing them. used when cutting a head.
	int getIndex() const;

	static void resetHeadCount();
	static int headCount;
};
