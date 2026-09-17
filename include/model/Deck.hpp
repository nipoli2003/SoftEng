#pragma once

#include "Card.hpp"
#include <vector>
#include <optional>
#include <random>

class Deck {
public:
    Deck() = default;
    explicit Deck(std::vector<Card> cards) : m_cards(std::move(cards)) {}

    void shuffle(std::mt19937& rng);
    void shuffle();

    [[nodiscard]] std::optional<Card> draw();
    [[nodiscard]] bool empty() const noexcept { return m_cards.empty(); }
    [[nodiscard]] std::size_t remainingCards() const noexcept { return m_cards.size(); }
    [[nodiscard]] const std::vector<Card>& cards() const noexcept { return m_cards; }

    void addCard(const Card& card) { m_cards.push_back(card); }

private:
    std::vector<Card> m_cards;
};

namespace DeckFactory {
    Deck createItalian40();
    Deck createFrench52();
    Deck createDoubleFrenchWithJokers(int jokersPerDeck = 2);
    Deck createMultiDeck(int deckCount, bool includeJokers);
}