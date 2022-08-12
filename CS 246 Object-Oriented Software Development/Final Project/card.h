#pragma once

#include <string>
#include <iostream>

class BadCardException{};
class BadCardSuitException : public BadCardException{};
class BadCardValueException : public BadCardException{};

class Card
{
protected:
	// Here I considered using enum types for suit and value to get rid of the extra checking methods.
	// However, with enums I still need to match each input with their corresponding enum values, and
	// same when outputting them, so I decided to simply use char and std::string with extra checkings. 

	char suit;
	std::string value;
	bool checkSuitNormal() const; // TRUE if suit is valid and is not a joker.
	bool checkValue(const std::string v) const; // TRUE if v is a valid value;
	Card(); // should only be used by Joker.

	friend std::ostream& operator<<(std::ostream& os, const Card& card);
public:
	Card(char suit, std::string value);
	Card(std::string suit, std::string value);
	char getSuit() const;
	virtual std::string getValue(bool setJokerValue = true);
	virtual int getIntegerValue(); // Return card value evaluated in integer range [1, 13].
	virtual void resetJokerValue(); // For Joker.
};
