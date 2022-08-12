#pragma once

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include "hydra.h"

class Player;
class Heads;

class HydraBasic : Hydra
{
	bool testingMode;
	bool smallerDeck; // only available in testing mode.
	std::unordered_map<std::string, int> names;
	std::vector<std::shared_ptr<Player>> players;
	std::shared_ptr<Heads> heads;
	std::vector<std::pair<std::string, std::string>> enhancementList;
	std::unordered_map<int, bool> enhancements;
	void initDrawPiles(int numPlayers); // Initialize deck consisting of n decks of cards, shuffle, and distribute them to each player.
	void turn(const int playerIndex); // Process a player's turn. 
	void cutHead(std::shared_ptr<Player>& currPlayer); // currPlayer cuts a head.
	int promptNumPlayers() const;
	void checkInputString(const std::string& s) const; // Checks if s matches any quitting command.
	void outputWelcomeMessage() const;
	void promptEnhancementChoices();
	void outputSplitter() const;
	void notifyGameTermination() const;
	void notifyTurnBegins(const std::shared_ptr<Player>& currPlayer) const;
	void outputStats(int currPlayerIndex = -1, int currPlayerRemaining = -1) const;
	void outputBadHeadIndexWarning() const;
	void outputActionResultMessage(const std::shared_ptr<Player> currPlayer, const int actionNum, const int actionResult) const;
	void outputEnhancements() const;
public:
	HydraBasic(bool testingMode); // Initialize the game and prompt the number of players.
	virtual void play() override; 
};
