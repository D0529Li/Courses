#pragma once

#include "joker.h"
#include <memory>
#include <vector>

class DrawingFromEmptyPileException{}; // This is thrown when trying to draw from an empty pile.

class Pile
{
	std::vector<std::shared_ptr<Card>> cards;
public:
	Pile(const std::vector<std::shared_ptr<Card>>& newCards);
	Pile();
	void addCard(const std::shared_ptr<Card>& card);
	std::shared_ptr<Card> draw();
	void shuffle();
	int getSize() const;
};
