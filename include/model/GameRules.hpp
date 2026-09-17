#pragma once
#include "Card.hpp"
#include "Player.hpp"
#include <vector>

class GameRules {
    public:
        virtual ~GameRules() = default;

        // Checks whether a chosen card is legal to play
        [[nodiscard]] virtual bool isValidMove(const Card& card, const Hand& playerHand) const = 0;

        // Evaluates a completed trick: returns the index of the winning card in 'trick'
        [[nodiscard]] virtual std::size_t evaluateTrick(const std::vector<Card>& trick) const = 0;

        // Point scoring
        [[nodiscard]] virtual int calculateCardPoints(const Card& card) const = 0;
        [[nodiscard]] virtual int calculateTotalScore(const std::vector<Card>& capturedCards) const = 0;
};