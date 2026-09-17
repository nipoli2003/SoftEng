#include "model/BriscolaRules.hpp"

BriscolaRules::BriscolaRules(Suit trumpSuit)
    : m_trumpSuit(trumpSuit) {}

bool BriscolaRules::isValidMove(const Card& card, const Hand& playerHand) const {
    for (const auto& c : playerHand.cards()) {
        if (c.rank() == card.rank() && c.suit() == card.suit()) {
            return true;
        }
    }
    return false;
}

int BriscolaRules::calculateCardPoints(const Card& card) const {
    switch (card.rank()) {
        case Rank::Ace:            return 11;
        case Rank::Three:          return 10;
        case Rank::King_Re:        return 4;
        case Rank::Queen_Regina:   return 3;
        case Rank::Jack_Fante:     return 2;
        default:                   return 0;
    }
}

int BriscolaRules::calculateTotalScore(const std::vector<Card>& capturedCards) const {
    int total = 0;
    for (const auto& card : capturedCards) {
        total += calculateCardPoints(card);
    }
    return total;
}

int BriscolaRules::rankStrength(Rank rank) const noexcept {
    switch (rank) {
        case Rank::Ace:            return 10;
        case Rank::Three:          return 9;
        case Rank::King_Re:        return 8;
        case Rank::Queen_Regina:   return 7;
        case Rank::Jack_Fante:     return 6;
        case Rank::Seven:          return 5;
        case Rank::Six:            return 4;
        case Rank::Five:           return 3;
        case Rank::Four:           return 2;
        case Rank::Two:            return 1;
        default:                   return 0;
    }
}

std::size_t BriscolaRules::evaluateTrick(const std::vector<Card>& trick) const {
    if (trick.empty()) return 0;

    std::size_t winningIndex = 0;
    Suit leadSuit = trick[0].suit();

    for (std::size_t i = 1; i < trick.size(); ++i) {
        const Card& currentWinner = trick[winningIndex];
        const Card& challenger = trick[i];

        if (challenger.suit() == m_trumpSuit) {
            // Trump beats non-trump, or higher trump beats lower trump
            if (currentWinner.suit() != m_trumpSuit ||
                rankStrength(challenger.rank()) > rankStrength(currentWinner.rank())) {
                winningIndex = i;
            }
        } else if (currentWinner.suit() != m_trumpSuit && challenger.suit() == leadSuit) {
            // If neither is trump, higher card of the lead suit wins
            if (rankStrength(challenger.rank()) > rankStrength(currentWinner.rank())) {
                winningIndex = i;
            }
        }
    }

    return winningIndex;
}

std::map<int, int> BriscolaRules::calculateTeamScores(const std::vector<Player>& players) const {
    std::map<int, int> teamScores;
    for (const auto& player : players) {
        teamScores[player.teamId()] += calculateTotalScore(player.capturedCards());
    }
    return teamScores;
}