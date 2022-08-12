#include "hydra_basic.h"
#include "computer_player.h"
#include "heads.h"
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <Windows.h>    // Sleep

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

class DisconnectingPlayerException {};
class ReconnectingPlayerException {};

const int enhancement_grammar_index = 1;
const int enhancement_house_rule_index = 2;
const int enhancement_customized_names_index = 3;
const int enhancement_computer_players_index = 4;
const int enhancement_smaller_deck_index = 5;

void HydraBasic::initDrawPiles(int n)
{
	std::vector<std::shared_ptr<Card>> deck;
	// add n decks of cards to deck
	int deckSize;
	if (enhancements[enhancement_smaller_deck_index] == 1) {
		deckSize = 8;
		for (int i = 0; i < n; i++)
		{
			deck.emplace_back(std::make_shared<Card>('S', "A"));
			deck.emplace_back(std::make_shared<Card>('S', "4"));
			deck.emplace_back(std::make_shared<Card>('S', "8"));
			deck.emplace_back(std::make_shared<Card>('D', "A"));
			deck.emplace_back(std::make_shared<Card>('D', "4"));
			deck.emplace_back(std::make_shared<Card>('D', "8"));
			deck.emplace_back(std::make_shared<Joker>());
			deck.emplace_back(std::make_shared<Joker>());
		}
	}
	else {
		deckSize = 54;
		for (int i = 0; i < n; i++)
		{
			deck.emplace_back(std::make_shared<Card>('S', "A"));
			deck.emplace_back(std::make_shared<Card>('S', "2"));
			deck.emplace_back(std::make_shared<Card>('S', "3"));
			deck.emplace_back(std::make_shared<Card>('S', "4"));
			deck.emplace_back(std::make_shared<Card>('S', "5"));
			deck.emplace_back(std::make_shared<Card>('S', "6"));
			deck.emplace_back(std::make_shared<Card>('S', "7"));
			deck.emplace_back(std::make_shared<Card>('S', "8"));
			deck.emplace_back(std::make_shared<Card>('S', "9"));
			deck.emplace_back(std::make_shared<Card>('S', "10"));
			deck.emplace_back(std::make_shared<Card>('S', "J"));
			deck.emplace_back(std::make_shared<Card>('S', "Q"));
			deck.emplace_back(std::make_shared<Card>('S', "K"));
			deck.emplace_back(std::make_shared<Card>('H', "A"));
			deck.emplace_back(std::make_shared<Card>('H', "2"));
			deck.emplace_back(std::make_shared<Card>('H', "3"));
			deck.emplace_back(std::make_shared<Card>('H', "4"));
			deck.emplace_back(std::make_shared<Card>('H', "5"));
			deck.emplace_back(std::make_shared<Card>('H', "6"));
			deck.emplace_back(std::make_shared<Card>('H', "7"));
			deck.emplace_back(std::make_shared<Card>('H', "8"));
			deck.emplace_back(std::make_shared<Card>('H', "9"));
			deck.emplace_back(std::make_shared<Card>('H', "10"));
			deck.emplace_back(std::make_shared<Card>('H', "J"));
			deck.emplace_back(std::make_shared<Card>('H', "Q"));
			deck.emplace_back(std::make_shared<Card>('H', "K"));
			deck.emplace_back(std::make_shared<Card>('H', "A"));
			deck.emplace_back(std::make_shared<Card>('C', "2"));
			deck.emplace_back(std::make_shared<Card>('C', "3"));
			deck.emplace_back(std::make_shared<Card>('C', "4"));
			deck.emplace_back(std::make_shared<Card>('C', "5"));
			deck.emplace_back(std::make_shared<Card>('C', "6"));
			deck.emplace_back(std::make_shared<Card>('C', "7"));
			deck.emplace_back(std::make_shared<Card>('C', "8"));
			deck.emplace_back(std::make_shared<Card>('C', "9"));
			deck.emplace_back(std::make_shared<Card>('C', "10"));
			deck.emplace_back(std::make_shared<Card>('C', "J"));
			deck.emplace_back(std::make_shared<Card>('C', "Q"));
			deck.emplace_back(std::make_shared<Card>('C', "K"));
			deck.emplace_back(std::make_shared<Card>('D', "A"));
			deck.emplace_back(std::make_shared<Card>('D', "2"));
			deck.emplace_back(std::make_shared<Card>('D', "3"));
			deck.emplace_back(std::make_shared<Card>('D', "4"));
			deck.emplace_back(std::make_shared<Card>('D', "5"));
			deck.emplace_back(std::make_shared<Card>('D', "6"));
			deck.emplace_back(std::make_shared<Card>('D', "7"));
			deck.emplace_back(std::make_shared<Card>('D', "8"));
			deck.emplace_back(std::make_shared<Card>('D', "9"));
			deck.emplace_back(std::make_shared<Card>('D', "10"));
			deck.emplace_back(std::make_shared<Card>('D', "J"));
			deck.emplace_back(std::make_shared<Card>('D', "Q"));
			deck.emplace_back(std::make_shared<Card>('D', "K"));
			deck.emplace_back(std::make_shared<Joker>());
			deck.emplace_back(std::make_shared<Joker>());
		}
	}
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(deck.begin(), deck.end(), std::default_random_engine(seed));
	auto iter = deck.begin();
	for (auto player : players) {
		std::vector<std::shared_ptr<Card>> currDeck;
		for (int i = 0; i < deckSize; i++) {
			currDeck.push_back(*iter);
			iter++;
		}
		player->acceptDrawPile(currDeck);
	}
}

HydraBasic::HydraBasic(bool testingMode) : testingMode{ testingMode }
{
	outputWelcomeMessage();
	// Initialize enhancement list.
	enhancementList.push_back(std::make_pair("Grammar", "Improve grammar accuracy"));
	enhancementList.push_back(std::make_pair("House Rule", "Players can cut a head whenever they want by typing in \"cut\""));
	enhancementList.push_back(std::make_pair("Customized Name", "Players can customize their unique names"));
	enhancementList.push_back(std::make_pair("Computer Players", "Replace a player with a computer player by typing \"disconnect\";\n\t Reconnect a player by typing \"reconnect\""));
	enhancementList.push_back(std::make_pair("Smaller Decks", "Use a smaller deck with 8 cards [AS, 4S, 8S, AD, 4D, 8D, Joker, Joker] each"));
	enhancements[1] = false;
	enhancements[2] = false;
	enhancements[3] = false;
	enhancements[4] = false;
	enhancements[5] = false;
	promptEnhancementChoices();
}

void HydraBasic::cutHead(std::shared_ptr<Player>& currPlayer)
{
	std::vector<std::shared_ptr<Card>> headCards;
	currPlayer->cutHead(heads->popHead()->popAllCards(), headCards);
	for (size_t i = 0; i < headCards.size(); i++) { // Create 2 new heads.
		heads->createNewHead(headCards[i]);
	}
}

int HydraBasic::promptNumPlayers() const
{
	std::string s;
	int numPlayers;
	while (true) {
		cout << endl << "How many players?" << endl;
		getline(cin, s);
		try {
			checkInputString(s);
			numPlayers = std::stoi(s);
		}
		catch (std::invalid_argument) {
			cout << "Please enter an integer value at least 2." << endl << endl;
			continue;
		}
		break;
	}
	return numPlayers;
}

void HydraBasic::checkInputString(const string& s) const
{
	if (s == "quit") throw QuitGameException();
	if (enhancements.at(enhancement_computer_players_index)) {
		if (s == "disconnect") throw DisconnectingPlayerException();
		if (s == "reconnect") throw ReconnectingPlayerException();
	}
}

void HydraBasic::outputWelcomeMessage() const
{
	outputSplitter();
	cout << "Welcome to the Hydra game designed by Yuchen Li! " << endl << endl
		<< "Here are some tips for you before playing the game." << endl << endl
		<< "1. Before starting a new game, you can choose what enhancement(s) you want to add." << endl
		<< "\t Once chosen, the enhancements cannot be changed unless you start another game, " << endl
		<< "\t at which time you will be able to choose enhancement(s) for that. " << endl << endl
		<< "2. You can quit the current game manually by typing in \"quit\" at any time while in an active game." << endl << endl
		<< "3. You can type in \"q\" while outside a game to quit the program." << endl << endl;
	cout << "Press Enter to select enhancement(s) for the next game..." << endl;
	string useless;
	getline(cin, useless);

}

void HydraBasic::promptEnhancementChoices()
{
	outputSplitter();
	cout << "Do you want any enhancement for this game? (Y/N)" << endl;
	string s;
	while (true) {
		getline(cin, s);
		checkInputString(s);
		if (s.length() > 1) continue;
		if (toupper(s[0]) == 'Y') {
			outputEnhancements();
			cout << endl << "Enter the number of enhancement you wish to add [one per line] followed by line \"0\"." << endl;
			string str;
			while (true) {
				getline(cin, str);
				checkInputString(str);
				int enhancementNum;
				try {
					enhancementNum = stoi(str);
				}
				catch (...) {
					continue;
				}
				if (enhancementNum == 0) break;
				if (enhancementNum < 0 || enhancementNum > enhancementList.size()) {
					cout << endl << enhancementNum << " is not a valid number of enhancement." << endl;
					continue;
				}
				cout << endl << "Enhancement \"" << enhancementList[enhancementNum - 1].first << "\" is switched on!" << endl;
				enhancements[enhancementNum] = true;
			}
			break;
		}
		if (toupper(s[0]) == 'N') break;
	}
}

void HydraBasic::notifyTurnBegins(const std::shared_ptr<Player>& currPlayer) const
{
	cout << endl << currPlayer->getName() << ", it is your turn." << endl;
	std::string s;
	getline(cin, s);
	checkInputString(s);
}

void HydraBasic::outputSplitter() const
{
	cout << endl << "----------------------------------------------------------------------" << endl << endl;
}

void HydraBasic::notifyGameTermination() const
{
	cout << endl << "The current game has been terminated. " << endl << endl;
}

void HydraBasic::outputStats(int currPlayerIndex, int currPlayerRemaining) const
{
	outputSplitter();
	cout << *heads << endl;
	cout << "Players:" << endl;
	for (size_t i = 0; i < players.size(); i++) {
		cout << *players[i];
		if (i == currPlayerIndex) {
			cout << " + " << (players[i]->getCurrCard() == nullptr ? 0 : 1) << " in hand, " << currPlayerRemaining - 1 << " remaining, "
				<< (players[i]->getReserve() == nullptr ? 0 : 1) << " in reserve";
		}
		cout << endl;
	}
}

void HydraBasic::outputBadHeadIndexWarning() const
{
	auto indexPair = heads->getHeadIndexRange();
	cout << endl << "You entered an invalid index of head. A valid index is between " << indexPair.first
		<< " and " << indexPair.second << ". " << endl << "Please enter again." << endl;
	Sleep(2000);
}

void HydraBasic::outputActionResultMessage(const std::shared_ptr<Player> currPlayer, const int actionNum, const int actionResult) const
{
	int sleepDuration = 1000;
	cout << endl;
	std::string message;
	switch (actionResult) {
	case 0: cout << currPlayer->getName() << ", the card " << *currPlayer->getCurrCard()
		<< " in your hand cannot be added to head " << actionNum << ". Please enter again. " << endl; break;
	case 1: cout << "Card " << *currPlayer->getCurrCard() << " is added successfully to head " << actionNum << "! " << endl; break;
	case 2: cout << "Card " << *currPlayer->getCurrCard() << " is added successfully to head " << actionNum << "! " << endl <<
		currPlayer->getName() << ", your current turn will end immediately." << endl; break;
	case 3: cout << "Card " << *currPlayer->getCurrCard() << " is added successfully to head " << actionNum << " on top of an Ace! " << endl; break;
	case 4: cout << currPlayer->getName() << ", the card " << *currPlayer->getCurrCard()
		<< " in your hand cannot be added to head " << actionNum << ". " << endl
		<< "You have to cut a head because that is your only legal move. " << endl; sleepDuration *= 2; break;
	}
	Sleep(sleepDuration);
}

void HydraBasic::outputEnhancements() const
{
	cout << endl << "All enhancements are listed below with their current status. " << endl << endl;
	int i = 1;
	for (auto enhancement : enhancementList) {
		try {
			cout << i << ". " << enhancement.first << " (" << enhancement.second << ") - STATUS: "
				<< (enhancements.at(i) == true ? "ON" : "OFF") << endl;
			i++;
		}
		catch (...) {
			cout << "Something is wrong with my code." << endl; // SHOULD BE TESTED BEFORE SUBMITTION.
		}
	}
}

void HydraBasic::turn(const int playerIndex)
{
	std::shared_ptr<Player>& currPlayer = players[playerIndex];
	while (true) {
		try {
			outputStats();
			notifyTurnBegins(currPlayer);
			break;
		}
		catch (DisconnectingPlayerException) {
			cout << endl << "Enter the name of the disconnecting player." << endl;
			string s;
			while (true) {
				getline(cin, s);
				if (names[s] == 1) break;
				cout << endl << "The name you entered is not one of the players. Please enter again." << endl;
			}
			std::vector<std::shared_ptr<Player>>::iterator it;
			for (it = players.begin(); it != players.end(); it++) {
				if ((*it)->getName() == s) break;
			}
			if ((*it)->isComputer()) {
				cout << (*it)->getName() << " has already disconnected. " << endl;
			}
			else {
				std::vector<std::shared_ptr<Player>> newPlayers;
				newPlayers.assign(players.begin(), it);
				auto newComputerPlayer = std::make_shared<ComputerPlayer>();
				(*it)->transferInfo(newComputerPlayer);
				newPlayers.push_back(newComputerPlayer);
				while (++it != players.end()) {
					newPlayers.push_back(*it);
				}
				players = newPlayers;
			}
			cout << endl << s << " is now replaced with a computer player! " << endl;
			Sleep(500);
		}
		catch (ReconnectingPlayerException) {
			cout << endl << "Enter the name of the reconnecting player." << endl;
			string s;
			while (true) {
				getline(cin, s);
				if (names[s] == 1) break;
				cout << endl << "The name you entered is not one of the players. Please enter again." << endl;
			}
			std::vector<std::shared_ptr<Player>>::iterator it;
			for (it = players.begin(); it != players.end(); it++) {
				if ((*it)->getName() == s) break;
			}
			if (!(*it)->isComputer()) {
				cout << (*it)->getName() << " is not disconnected. " << endl;
			}
			else {
				std::vector<std::shared_ptr<Player>> newPlayers;
				newPlayers.assign(players.begin(), it);
				auto newPlayer = std::make_shared<Player>();
				(*it)->transferInfo(newPlayer);
				newPlayers.push_back(newPlayer);
				while (++it != players.end()) {
					newPlayers.push_back(*it);
				}
				players = newPlayers;
			}
			cout << endl << s << " is now back! " << endl;
			Sleep(500);
		}
	}
	int remaining = heads->getSize(); // Remaining number of cards that need to be drawn.
	while (remaining > 0) {
		// Draw a new card.
		// If draw pile is empty and discard pile is not empty,
		// then reshuffle the discard pile to form a new draw pile and draw from it.
		// If both draw pile and discard pile are empty, then add reserve to discard pile and end turn.
		if (currPlayer->draw() == 1) { // not a normal draw
			break;
		}
		// might need to check winning.

		// Keep prompting action number until a valid number of head is entered. 
		// Exchanging with reserve does not exit the loop.
		while (true) {
			outputStats(playerIndex, remaining);
			int actionNum = currPlayer->promptAction(heads, enhancements.at(enhancement_grammar_index),
				enhancements.at(enhancement_house_rule_index)); // The player inputs their choice of action.
			if (actionNum == 0) {
				// Exchange with or add to reserve, and output message for each case.  
				// Prompt again if exchanged with reserve. 
				if (heads->getSize() <= 1) {
					cout << endl << currPlayer->getName() << ", there is no more than 1 head, so you cannot use any reserve." << endl;
					Sleep(1000);
					continue;
				}
				if (currPlayer->exchangeReserve() == 1) continue;
			}
			else if (actionNum == -1) { // This is only reachable with house rule enabled.
				cout << endl << currPlayer->getName() << " is cutting a head..." << endl;
				Sleep(1000);
				cutHead(currPlayer);
				cout << endl << currPlayer->getName() << " has successfully cut a head!" << endl;
				Sleep(1000);
				remaining = 1;
			}
			else {
				int actionResult;
				try {
					actionResult = heads->addCardToHead(actionNum, currPlayer->getCurrCard());
				}
				catch (BadHeadIndexException) { // Invalid number of head. Re-prompt.
					outputBadHeadIndexWarning();
					continue;
				}
				if (actionResult == 4 && currPlayer->getReserve() == nullptr && heads->getSize() > 1) actionResult = 0;
				outputActionResultMessage(currPlayer, actionNum, actionResult);
				if (actionResult == 0) { // unable to add card; Re-prompt.
					currPlayer->getCurrCard()->resetJokerValue(); // Reset joker's value if it is a joker.
					continue;
				}
				else if (actionResult == 2) { // currPlayer will draw no more cards this turn.
					remaining = 1;
				}
				else if (actionResult == 4) { // unable to add card; need to cut off a head and create 2 new heads.
					cout << endl << currPlayer->getName() << " is cutting a head..." << endl;
					Sleep(1000);
					cutHead(currPlayer);
					cout << endl << currPlayer->getName() << " has successfully cut a head!" << endl;
					Sleep(1000);
					remaining = 1;
				}
			}
			break; // If it reaches here, it means a valid "move" has been made. Will not prompt again for the current move.
		} // current prompt
		currPlayer->checkWin();
		remaining--;
	} // current player's draw phase
	cout << endl << currPlayer->getName() << " is ending their turn..." << endl;
	Sleep(1000);
	currPlayer->endTurn(testingMode);
	cout << endl << currPlayer->getName() << " has ended their turn." << endl;
	Sleep(1000);
}

void HydraBasic::play()
{
	try {
		outputSplitter();
		cout << endl << "A new game has been made active!" << endl << endl;
		outputEnhancements();
		int numPlayers = promptNumPlayers();
		// Initialize each player with their own name.
		for (int i = 0; i < numPlayers; i++)
		{
			string name;
			if (enhancements[enhancement_customized_names_index]) {
				cout << endl << "Player " << i + 1 << "'s name? " << endl;
				while (true) {
					getline(cin, name);
					if (name == "") {
						cout << "The name cannot be empty! Enter again." << endl;
						continue;
					}
					if (names[name] == 0) {
						break;
					}
					cout << "Duplicate name! Please enter an unique name for this player." << endl;
				}
			}
			else {
				name = "Player " + std::to_string(i + 1);
			}
			names[name] = 1;
			checkInputString(name);			
			players.emplace_back(std::make_shared<Player>(testingMode, (enhancements[enhancement_smaller_deck_index] == 1) ? true : false, name));
		}
		// Initialize heads.
		heads = std::make_shared<Heads>();
		// Game begins.
		if (!testingMode) initDrawPiles(players.size());
		outputSplitter();
		cout << endl << "The game is starting! " << endl;
		Sleep(1000);
		outputSplitter();
		cout << endl << players[0]->getName() << " is creating the first head! " << endl;
		players[0]->draw();
		heads->createNewHead(players[0]->getCurrCard());
		// playerIndex refers to the index value of the vector i.e. the value in square brackets []. 
		// All players' names (they are defaulted to numbers i.e. Player 1, 2, etc..) are stored in class Player. 
		int currPlayerIndex = 1; // Player 0's first turn is trivially taken. 
		while (true) // The game continues until PlayerWonException is thrown or user-terminated.
		{
			currPlayerIndex %= players.size();
			turn(currPlayerIndex++);
		}
	}
	catch (QuitGameException) {
		outputSplitter();
		notifyGameTermination();
		return;
	}
	catch (playerWonException& e) {
		cout << endl << e.playerName << " wins!" << endl;
		outputSplitter();
		return;
	}
}
