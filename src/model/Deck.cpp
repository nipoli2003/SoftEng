#include "model/Deck.hpp"
#include <algorithm> // Necessary for std::ranges::shuffle
#include <random>    // Necessary for std::random_device and std::mt19937
#include <array>

void Deck::shuffle(std::mt19937& rng) {
    std::ranges::shuffle(m_cards, rng);
}

void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 rng(rd());
    shuffle(rng);
}

std::optional<Card> Deck::draw() {
    if (m_cards.empty()) {
        return std::nullopt;
    }
    Card dealt = m_cards.back();
    m_cards.pop_back();
    return dealt;
}

namespace DeckFactory {

Deck createItalian40() {
    std::vector<Card> cards;
    cards.reserve(40);

    constexpr std::array<Suit, 4> suits = {
        Suit::Bastoni_Clubs, Suit::Coppe_Hearts, Suit::Denari_Diamonds, Suit::Spade_Spades
    };

    // 1 to 7, plus Fante (Jack), Cavallo (Knight), and Re (King)
    constexpr std::array<Rank, 10> ranks = {
        Rank::Ace, Rank::Two, Rank::Three, Rank::Four, Rank::Five,
        Rank::Six, Rank::Seven, Rank::Jack_Fante, Rank::Queen_Regina, Rank::King_Re
    };

    for (auto suit : suits) {
        for (auto rank : ranks) {
            cards.emplace_back(rank, suit, 0);
        }
    }
    return Deck(std::move(cards));
}

Deck createFrench52() {
    std::vector<Card> cards;
    cards.reserve(52);

    constexpr std::array<Suit, 4> suits = {
        Suit::Bastoni_Clubs, Suit::Coppe_Hearts, Suit::Denari_Diamonds, Suit::Spade_Spades
    };

    constexpr std::array<Rank, 13> ranks = {
        Rank::Ace, Rank::Two, Rank::Three, Rank::Four, Rank::Five,
        Rank::Six, Rank::Seven, Rank::Eight, Rank::Nine, Rank::Ten,
        Rank::Jack_Fante, Rank::Queen_Regina, Rank::King_Re
    };

    for (auto suit : suits) {
        for (auto rank : ranks) {
            cards.emplace_back(rank, suit, 0);
        }
    }
    return Deck(std::move(cards));
}

Deck createDoubleFrenchWithJokers(int jokersPerDeck) {
    std::vector<Card> cards;
    cards.reserve((52 + jokersPerDeck) * 2);

    for (int deckId = 0; deckId < 2; ++deckId) {
        Deck singleDeck = createFrench52();
        for (const auto& card : singleDeck.cards()) {
            cards.emplace_back(card.rank(), card.suit(), deckId);
        }
        for (int j = 0; j < jokersPerDeck; ++j) {
            cards.emplace_back(Rank::Joker, Suit::None, deckId);
        }
    }
    return Deck(std::move(cards));
}

Deck createMultiDeck(int deckCount, bool includeJokers) {
    std::vector<Card> cards;
    int cardsPerDeck = includeJokers ? 54 : 52;
    cards.reserve(deckCount * cardsPerDeck);

    for (int deckId = 0; deckId < deckCount; ++deckId) {
        Deck singleDeck = createFrench52();
        for (const auto& card : singleDeck.cards()) {
            cards.emplace_back(card.rank(), card.suit(), deckId);
        }
        if (includeJokers) {
            cards.emplace_back(Rank::Joker, Suit::None, deckId);
            cards.emplace_back(Rank::Joker, Suit::None, deckId);
        }
    }
    return Deck(std::move(cards));
}

} // namespace DeckFactory