#include "pile.h"
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

Pile::Pile(const std::vector<std::shared_ptr<Card>>& newCards)
{
	for (auto card : newCards) {
		cards.push_back(card);
	}
}

Pile::Pile() {}

void Pile::addCard(const std::shared_ptr<Card>& card)
{
	cards.push_back(card);
}

std::shared_ptr<Card> Pile::draw()
{
	if (cards.empty()) throw DrawingFromEmptyPileException();
	auto card = cards.at(0);
	cards.erase(cards.begin());
	return card;
}

void Pile::shuffle()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

int Pile::getSize() const
{
	return cards.size();
}
