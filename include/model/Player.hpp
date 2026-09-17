#pragma once
#include <string>
#include <vector>
#include <optional>
#include "Card.hpp"
#include "Hand.hpp"

class Player {
public:
    Player(int id, std::string name, bool isAI = false, int teamId = -1);

    [[nodiscard]] int id() const noexcept { return m_id; }
    [[nodiscard]] int teamId() const noexcept { return m_teamId; }
    [[nodiscard]] const std::string& name() const noexcept { return m_name; }
    [[nodiscard]] bool isAI() const noexcept { return m_isAI; }

    // Hand interactions
    Hand& hand() noexcept { return m_hand; }
    [[nodiscard]] const Hand& hand() const noexcept { return m_hand; }

    // Trick / Capture pile management
    void captureCard(const Card& card);
    void captureCards(const std::vector<Card>& cards);
    [[nodiscard]] const std::vector<Card>& capturedCards() const noexcept { return m_capturedCards; }

    // Cleans up state between deals/rounds
    void resetForNewRound() noexcept;

private:
    int m_id;
    int m_teamId;
    std::string m_name;
    bool m_isAI;
    Hand m_hand;
    std::vector<Card> m_capturedCards;
};