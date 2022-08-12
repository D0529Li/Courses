#include "card.h"

bool Card::checkSuitNormal() const
{
	if (suit != 'S' && suit != 'H' && suit != 'C' && suit != 'D') return false;
	return true;
}

bool Card::checkValue(const std::string v) const
{
	if (v != "A" && v != "2" && v != "3" && v != "4" && v != "5" && v != "6" && v != "7" &&
		v != "8" && v != "9" && v != "10" && v != "J" && v != "Q" && v != "K") return false;
	return true;
}

Card::Card() {}

Card::Card(char suit, std::string value) : suit{ suit }, value{ value }
{
	if (!checkSuitNormal()) throw BadCardSuitException();
	if (!checkValue(value)) throw BadCardValueException();
}

Card::Card(std::string suit, std::string value) : suit{ suit[0] }, value{ value }
{
	if (suit.size() > 1 || !checkSuitNormal()) throw BadCardSuitException();
	if (!checkValue(value)) throw BadCardValueException();
}

char Card::getSuit() const
{
	return suit;
}

std::string Card::getValue(bool setJokerValue)
{
	return value;
}

int Card::getIntegerValue()
{
	if (value == "A") return 1;
	if (value == "J") return 11;
	if (value == "Q") return 12;
	if (value == "K") return 13;
	return std::stoi(value);
}

void Card::resetJokerValue() {}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
	os << card.value;
	if (card.value != "Joker") {
		os << card.suit;
	}
	return os;
}
