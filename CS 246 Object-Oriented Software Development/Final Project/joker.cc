#include "joker.h"
#include <iostream>

bool Joker::checkValueUnset() const
{
	return (value == "Joker");
}

void Joker::promptAndSetJokerValue()
{
	if (checkValueUnset()) {
		std::string s;
		while (true) { // Keep prompting joker value until a valid value is entered.
			std::cout << "Joker value?" << std::endl;
			std::getline(std::cin, s);
			if (checkValue(s)) {
				value = s;
				break;
			}
		}
	}
}

Joker::Joker() : Card() {
	suit = 'J';
	value = "Joker";
}

std::string Joker::getValue(bool setJokerValue)
{
	if (setJokerValue && checkValueUnset()) {
		promptAndSetJokerValue();
	}
	return value;
}

int Joker::getIntegerValue()
{
	if (checkValueUnset()) {
		promptAndSetJokerValue();
	}
	if (value == "A") return 1;
	if (value == "J") return 11;
	if (value == "Q") return 12;
	if (value == "K") return 13;
	return std::stoi(value);
}

void Joker::resetJokerValue()
{
	value = "Joker";
}

void Joker::setValue(const std::string& newValue)
{
	if (!checkValue(newValue)) {
		throw BadCardValueException();
	}
	value = newValue;
}
