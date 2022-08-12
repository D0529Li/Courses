#pragma once
#include "card.h"

class Joker : public Card
{
	bool checkValueUnset() const; // TRUE if the value of the joker is not set.
	void promptAndSetJokerValue();
public:
	Joker();
	virtual std::string getValue(bool setJokerValue = true) override; // If the value is not set for the joker, then prompt. 
	virtual int getIntegerValue() override;
	virtual void resetJokerValue() override; // Reset the value back to "Joker".
	void setValue(const std::string& newValue); // Prompt the value of this joker and set.
};
