#include "head.h"
#include "joker.h"

Head::Head() : index{ ++headCount } {}

Head::Head(std::shared_ptr<Card> card) : Head()
{
	if (card->getValue(false) == "Joker") {
		// Set joker value to "2". 
		std::static_pointer_cast<Joker>(card)->setValue("2");
	}
	cards.push_back(card);
}

Head::Head(const Head& otherHead) : index{otherHead.index}
{
	for (std::shared_ptr<Card> card : otherHead.cards) {
		cards.push_back(card);
	}
}

// return value: 1 - card added;
//				 2 - card added and no further draws permitted;
//				 3 - Ace on the top and the card is added on it
//				 0 - cannot add the card to the head [card not added]
int Head::addCard(std::shared_ptr<Card> card)
{
	auto topCard = cards[cards.size() - 1];
	if (topCard->getValue() == "A") {
		cards.push_back(card);
		if (card->getValue() == "A") return 2;
		return 3;
	}
	else {
		if (card->getIntegerValue() <= topCard->getIntegerValue()) {
			cards.push_back(card);
			if (card->getIntegerValue() < topCard->getIntegerValue()) return 1;
			return 2;
		}
	}
	return 0;
}

std::vector<std::shared_ptr<Card>> Head::popAllCards()
{
	std::vector<std::shared_ptr<Card>> res;
	for (std::shared_ptr<Card> card : cards) {
		res.push_back(card);
	}
	return res;
}

int Head::getIndex() const
{
	return index;
}

void Head::resetHeadCount()
{
	headCount = 0;
}

int Head::headCount = 0;

std::ostream& operator<<(std::ostream& os, const Head& head)
{
	os << head.index << ": " << *(head.cards[head.cards.size() - 1]) << " (" << head.cards.size() << ")";
	return os;
}
