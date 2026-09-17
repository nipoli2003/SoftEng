#pragma once
#include <string>
#include <string_view>
#include <compare>

enum class Suit {
    // Italian Regional / Anglo-French mapping
    Bastoni_Clubs,
    Coppe_Hearts,
    Denari_Diamonds,
    Spade_Spades,
    None // For Jokers
};

enum class Rank {
    Ace = 1,
    Two, Three, Four, Five, Six, Seven,
    Eight, Nine, Ten,
    Jack_Fante,       // 8 in Italian deck, 11 in Standard
    Knight_Cavallo,   // 9 in Italian deck
    Queen_Regina,     // 12 in Standard deck
    King_Re,          // 10 in Italian deck, 13 in Standard
    Joker
};

class Card {
public:
    Card(Rank rank, Suit suit, int deckOrigin = 0)
        : m_rank(rank), m_suit(suit), m_deckOrigin(deckOrigin) {}

    [[nodiscard]] Rank rank() const noexcept { return m_rank; }
    [[nodiscard]] Suit suit() const noexcept { return m_suit; }
    [[nodiscard]] int deckOrigin() const noexcept { return m_deckOrigin; }
    [[nodiscard]] bool isJoker() const noexcept { return m_rank == Rank::Joker; }

    // Three-way comparison (C++20 spaceship operator)
    // Explicit comparison operators (compatible with all Apple Clang versions)
    bool operator==(const Card& other) const noexcept {
        return m_rank == other.m_rank &&
               m_suit == other.m_suit &&
               m_deckOrigin == other.m_deckOrigin;
    }

    bool operator<(const Card& other) const noexcept {
        if (m_deckOrigin != other.m_deckOrigin) return m_deckOrigin < other.m_deckOrigin;
        if (m_suit != other.m_suit) return static_cast<int>(m_suit) < static_cast<int>(other.m_suit);
        return static_cast<int>(m_rank) < static_cast<int>(other.m_rank);
    }

    [[nodiscard]] std::string toString() const;

private:
    Rank m_rank;
    Suit m_suit;
    int m_deckOrigin; // Differentiates identical cards in multi-deck games
};