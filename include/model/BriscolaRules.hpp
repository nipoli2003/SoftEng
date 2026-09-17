#pragma once
#include "GameRules.hpp"
#include <map>

class BriscolaRules : public GameRules {
    public:
        explicit BriscolaRules(Suit trumpSuit);

        void setTrumpSuit(Suit trumpSuit) noexcept { m_trumpSuit = trumpSuit; }
        [[nodiscard]] Suit trumpSuit() const noexcept { return m_trumpSuit; }

        // Any card in the player's hand is legal in Briscola (no requirement to follow suit)
        [[nodiscard]] bool isValidMove(const Card& card, const Hand& playerHand) const override;

        // Returns index in 'trick' of the card that won the trick
        [[nodiscard]] std::size_t evaluateTrick(const std::vector<Card>& trick) const override;

        [[nodiscard]] int calculateCardPoints(const Card& card) const override;
        [[nodiscard]] int calculateTotalScore(const std::vector<Card>& capturedCards) const override;

        // Team aggregate helper (works for 1v1 and 2v2)
        [[nodiscard]] std::map<int, int> calculateTeamScores(const std::vector<Player>& players) const;

    private:
        Suit m_trumpSuit;
        [[nodiscard]] int rankStrength(Rank rank) const noexcept;
};