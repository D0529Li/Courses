#include "player.h"
#include <iostream>
#include <Windows.h>
#include <exception>

const int sleepDuration = 1000;

void Player::reshuffle()
{
	std::cout << std::endl << "[reshuffling to form a new draw pile...]" << std::endl;
	Sleep(sleepDuration);
	while (true) {
		try {
			auto card = pImpl->discardPile->draw();
			pImpl->drawPile->addCard(card);
		}
		catch (DrawingFromEmptyPileException) {
			pImpl->drawPile->shuffle();
			break;
		}
	}
	std::cout << "[reshuffle complete!] " << std::endl;
	Sleep(sleepDuration);
}

Player::Player() {
	pImpl = std::make_shared<PlayerImpl>();
}

Player::Player(bool testingMode, bool smallDeck, std::string name, bool computerPlayer)
{
	pImpl = std::make_shared<PlayerImpl>();
	pImpl->testingMode = testingMode;
	pImpl->computerPlayer = computerPlayer;
	pImpl->name = name;
	if (testingMode) {
		if(smallDeck) pImpl->drawPileRemaining = 8;
		else pImpl->drawPileRemaining = 54;
		pImpl->discardPileRemaining = 0;
	}
}

void Player::acceptDrawPile(const std::vector<std::shared_ptr<Card>>& pile)
{
	for (std::shared_ptr<Card> card : pile) {
		pImpl->drawPile->addCard(card);
	}
}

std::string Player::getName() const
{
	return pImpl->name;
}

std::shared_ptr<Card> Player::getReserve() const
{
	return pImpl->reserve;
}

bool Player::isComputer() const
{
	return pImpl->computerPlayer;
}

void Player::addToDiscardPile(const std::vector<std::shared_ptr<Card>>& cards)
{
	for (std::shared_ptr<Card> card : cards) {
		pImpl->discardPile->addCard(card);
	}
	if (pImpl->testingMode) pImpl->discardPileRemaining += cards.size();
}

void Player::checkInputString(const std::string& s) const
{
	if (s == "quit") throw QuitGameException();
}

void Player::checkWin()
{
	// In testing mode 
	if (pImpl->testingMode) {
		if (pImpl->drawPileRemaining == 0 && pImpl->discardPileRemaining == 0 && pImpl->reserve == nullptr)
			throw playerWonException{ pImpl->name };
	}
	// In normal mode
	else if (pImpl->drawPile->getSize() == 0 && pImpl->discardPile->getSize() == 0 && pImpl->reserve == nullptr)
		throw playerWonException{ pImpl->name };
}

int Player::draw()
{
	// In testing mode 
	if (pImpl->testingMode) {
		if (pImpl->drawPileRemaining == 0) {
			checkWin();
			if (pImpl->discardPile->getSize() == 0) {
				pImpl->drawPileRemaining = 1; // add the reserve card to virtual draw pile.
				return 1;
			}
			pImpl->drawPileRemaining = pImpl->discardPileRemaining;
			pImpl->discardPileRemaining = 0;
		}
		promptTestingCard();
		pImpl->drawPileRemaining--;
		return 0;
	}
	// In normal mode
	try {
		pImpl->currCard = pImpl->drawPile->draw();
	}
	catch (DrawingFromEmptyPileException) {
		if (pImpl->discardPile->getSize() == 0) {
			checkWin();
			pImpl->discardPile->addCard(std::move(pImpl->reserve));
			return 1;
		}
		reshuffle();
		pImpl->currCard = pImpl->drawPile->draw();
	}
	return 0;
}

std::shared_ptr<Card> Player::getCurrCard()
{
	return pImpl->currCard;
}

int Player::exchangeReserve()
{
	using std::cout;
	using std::endl;
	cout << endl << getName() << " has ";
	std::swap(pImpl->currCard, pImpl->reserve);
	if (pImpl->currCard == nullptr) { // Added to reserve.
		cout << "added their card in hand to reserve. " << endl;
		return 0;
	}
	// Exchanged with reserve.
	cout << "exchanged their card in hand with the reserve. " << endl;
	return 1;
}

int Player::promptAction(const std::shared_ptr<Heads>& heads, const bool grammarEnhancement, const bool houseRuleEnhancement) const
{
	if (grammarEnhancement) {
		std::cout << std::endl << pImpl->name << ", you are holding ";
		if (pImpl->currCard->getValue(false) == "A" || pImpl->currCard->getValue(false) == "8") {
			std::cout << "an ";
		}
		else {
			std::cout << "a ";
		}
		std::cout << (*pImpl->currCard) << ". Your move?" << std::endl;
	}
	else {
		std::cout << std::endl << pImpl->name << ", you are holding a " << (*pImpl->currCard) << ". Your move?" << std::endl;
	}
	std::string s;
	int n;
	while (true) {
		try {
			std::getline(std::cin, s);
			checkInputString(s);
			if (houseRuleEnhancement) {
				if (s == "cut") return -1;
			}
			n = std::stoi(s);
		}
		catch (std::invalid_argument) {
			std::cout << "Please enter a valid value of action. " << std::endl;
			continue;
		}
		break;
	}
	return n;
}

void Player::acceptInfo(std::shared_ptr<PlayerImpl> from_pImpl)
{
	pImpl->testingMode = from_pImpl->testingMode;
	pImpl->name = from_pImpl->name;
	pImpl->computerPlayer = false;
	pImpl->drawPile = from_pImpl->drawPile;
	pImpl->discardPile = from_pImpl->discardPile;
	pImpl->currCard = from_pImpl->currCard;
	pImpl->reserve = from_pImpl->reserve;
	pImpl->drawPileRemaining = from_pImpl->drawPileRemaining;
	pImpl->discardPileRemaining = from_pImpl->discardPileRemaining;
}

void Player::transferInfo(std::shared_ptr<Player> to)
{
	to->acceptInfo(pImpl);
}

void Player::cutHead(const std::vector<std::shared_ptr<Card>>& cards, std::vector<std::shared_ptr<Card>>& headCards)
{
	pImpl->discardPile->addCard(std::move(pImpl->currCard));
	if (pImpl->reserve != nullptr) {
		pImpl->discardPile->addCard(std::move(pImpl->reserve));
	}
	addToDiscardPile(cards);
	std::cout << std::endl << "New heads will be formed by:" << std::endl;
	draw();
	std::cout << *pImpl->currCard << std::endl;
	headCards.push_back(pImpl->currCard);
	if (draw() == 1) throw playerWonException(pImpl->name);
	std::cout << *pImpl->currCard << std::endl;
	headCards.push_back(pImpl->currCard);
	Sleep(1000);
}

void Player::promptTestingCard()
{
	using std::cout;
	using std::cin;
	using std::endl;
	std::string newValue, newSuit;
	while (true) {
		try {
			cout << endl << "Card value?" << endl;
			getline(cin, newValue);
			checkInputString(newValue);
			if (newValue == "Joker") {
				auto newJoker = std::make_shared<Joker>();
				pImpl->currCard = newJoker;
			}
			else {
				cout << "Suit?" << endl;
				getline(cin, newSuit);
				checkInputString(newSuit);
				auto newCard = std::make_shared<Card>(newSuit, newValue);
				pImpl->currCard = newCard;
			}
		}
		catch (BadCardException) {
			cout << "Invalid card! " << endl
				<< "A valid value is one of \"A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K, Joker\"" << endl
				<< "and a valid suit is one of \"S, H, C, D\"" << endl;
			continue;
		}
		break;
	}

}

void Player::endTurn(bool testingMode)
{
	if (pImpl->reserve != nullptr) {
		std::cout << std::endl << "There is a reserve card. Adding it to discard pile..." << std::endl;
		Sleep(sleepDuration);
		if (testingMode) pImpl->discardPileRemaining++;
		else pImpl->discardPile->addCard(pImpl->reserve);
	}
	pImpl->currCard = nullptr;
	pImpl->reserve = nullptr;
}

std::ostream& operator<<(std::ostream& os, const Player& player)
{
	int drawPileSize = player.pImpl->testingMode ? player.pImpl->drawPileRemaining : player.pImpl->drawPile->getSize();
	int discardPileSize = player.pImpl->testingMode ? player.pImpl->discardPileRemaining : player.pImpl->discardPile->getSize();
	os << player.getName() << ": " << drawPileSize + discardPileSize << " ("
		<< drawPileSize << " draw, " << discardPileSize << " discard)";
	return os;
}
