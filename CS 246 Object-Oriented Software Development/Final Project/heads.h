#pragma once

#include "head.h"
#include <vector>

class BadHeadIndexException {}; // This is thrown when trying to add a card to a non-existing head (i.e. deleted head).

class Heads
{
	std::vector<std::shared_ptr<Head>> headCollection;
	bool checkHeadNumber(int n) const; // TRUE if n is a valid number of head. 
	int checkCutHead(const int headIndex, const std::shared_ptr<Card> card) const; // TRUE if the parameters lead to cutting a head. 

	friend std::ostream& operator<<(std::ostream& os, const Heads& heads);
public:
	Heads();
	void createNewHead(std::shared_ptr<Card> card); // Create a new head with one card.

	// return value: 1 - card added;
	//				 2 - card added and no further draws permitted; 
	//				 3 - Ace on the top and the card is added on it
	//				 0 - cannot add the card to the head [card not added]
	//				 4 - need to cut a head.
	int addCardToHead(int headIndex, std::shared_ptr<Card> card);

	std::shared_ptr<Head> popHead(); // erase the oldest head and return it. used when cutting a head.
	int determineFirstValidHeadIndex(std::shared_ptr<Card> card) const; // Used by ComputerPlayer. 
	int getSize() const;
	std::pair<int, int> getHeadIndexRange() const;
};
