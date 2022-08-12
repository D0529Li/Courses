#pragma once

#include "player.h"

class ComputerPlayer : public Player
{
public:
	ComputerPlayer();
	// A very basic and naive computer player.
	virtual int promptAction(const std::shared_ptr<Heads>& heads, const bool grammarEnhancement, const bool houseRuleEnhancement) const override;
	virtual void acceptInfo(std::shared_ptr<PlayerImpl> from_pImpl) override;
};
