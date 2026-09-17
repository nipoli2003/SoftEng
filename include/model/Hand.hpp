#pragma once
#include "model/Card.hpp"
#include <vector>
#include <optional>
#include <algorithm>

class Hand {
public:
    Hand() = default;

    void addCard(const Card& card) {
        m_cards.push_back(card);
    }

    [[nodiscard]] std::optional<Card> playCard(std::size_t index) {
        if (index >= m_cards.size()) {
            return std::nullopt;
        }
        Card card = m_cards[index];
        m_cards.erase(m_cards.begin() + index);
        return card;
    }

    [[nodiscard]] const std::vector<Card>& cards() const noexcept { return m_cards; }
    [[nodiscard]] std::size_t size() const noexcept { return m_cards.size(); }
    [[nodiscard]] bool empty() const noexcept { return m_cards.empty(); }

    void clear() noexcept { m_cards.clear(); }

    // Allows sorting hands by rank or suit
    template <typename Compare>
    void sort(Compare comp) {
        std::ranges::sort(m_cards, comp);
    }

private:
    std::vector<Card> m_cards;
};