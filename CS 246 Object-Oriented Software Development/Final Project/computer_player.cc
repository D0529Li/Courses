#include "computer_player.h"

ComputerPlayer::ComputerPlayer() : Player{} {}

int ComputerPlayer::promptAction(const std::shared_ptr<Heads>& heads, const bool grammarEnhancement, const bool houseRuleEnhancement) const
{
	// if (pImpl->currCard->getSuit() == 'J') return heads->getHeadIndexRange().first;
	return heads->determineFirstValidHeadIndex(pImpl->currCard);
}

void ComputerPlayer::acceptInfo(std::shared_ptr<PlayerImpl> from_pImpl)
{
	pImpl->testingMode = from_pImpl->testingMode;
	pImpl->name = from_pImpl->name;
	pImpl->computerPlayer = true;
	pImpl->drawPile = from_pImpl->drawPile;
	pImpl->discardPile = from_pImpl->discardPile;
	pImpl->currCard = from_pImpl->currCard;
	pImpl->reserve = from_pImpl->reserve;
	pImpl->drawPileRemaining = from_pImpl->drawPileRemaining;
	pImpl->discardPileRemaining = from_pImpl->discardPileRemaining;
}
