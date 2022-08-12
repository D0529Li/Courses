#pragma once

#include "pile.h"
#include "heads.h"

struct PlayerImpl {
	std::string name;
	bool testingMode;
	bool computerPlayer;
	std::shared_ptr<Pile> drawPile;
	std::shared_ptr<Pile> discardPile;
	std::shared_ptr<Card> currCard;
	std::shared_ptr<Card> reserve;
	int drawPileRemaining, discardPileRemaining; // only to be used in testing mode
	PlayerImpl() {
		drawPile = std::make_shared<Pile>();
		discardPile = std::make_shared<Pile>();
	}
};

class QuitGameException {}; // This is thrown when requesting to quit the current game.

class playerWonException { // This is thrown when some player has won the game.
public:
	const std::string playerName;
	playerWonException(std::string name) : playerName{ name } {}
};

class Player
{
	void reshuffle(); // Reshuffle the discard pile to form a new draw pile. Invoked when draw pile is empty and discard pile is not empty. 
	void addToDiscardPile(const std::vector<std::shared_ptr<Card>>& cards); // used when the player cuts a head. 
	void checkInputString(const std::string& s) const; // Checks if s matches any quitting command.

	friend std::ostream& operator<<(std::ostream& os, const Player& player);
protected:
	std::shared_ptr<PlayerImpl> pImpl;
public:
	Player();
	Player(bool testingMode, bool smallDeck, std::string name, bool computerPlayer = false);
	std::shared_ptr<Card> getCurrCard();
	void acceptDrawPile(const std::vector<std::shared_ptr<Card>>& pile); // Initialize the draw pile with the given vector.
	std::string getName() const;
	std::shared_ptr<Card> getReserve() const;
	bool isComputer() const;
	// Draw a new card from draw pile. Reshuffle if needed. 
	// Return value: 0 - Normally draw a card.
	//				 1 - The current turn needs to end immediately. 
	int draw();
	int exchangeReserve(); // Return 1 if EXCHANGED; 0 if ADDED to reserve. 
	void transferInfo(std::shared_ptr<Player> to); // Transfer info to another player.
	// Process cutHead. Add cards to discard pile and draw 2 cards from draw pile and add them to the reference parameter. 
	void cutHead(const std::vector<std::shared_ptr<Card>>& cards, std::vector<std::shared_ptr<Card>>& headCards);
	void promptTestingCard(); // In testing mode, prompt the drawing card.
	void checkWin(); // throw PlayerWonException if this player has won. 
	void endTurn(bool testingMode); // Clean-up when the current turn ends.

	// Prompt action from std::cin. 
	virtual int promptAction(const std::shared_ptr<Heads>& heads, const bool grammarEnhancement, const bool houseRuleEnhancement) const; 
	virtual void acceptInfo(std::shared_ptr<PlayerImpl> from_pImpl);
};
