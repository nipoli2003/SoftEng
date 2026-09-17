#pragma once

#include "GameEngine.hpp"
#include "model/Deck.hpp"
#include "model/Player.hpp"
#include "model/BriscolaRules.hpp"
#include <memory>
#include <vector>

class BriscolaEngine : public GameEngine {
public:
    // Setup for 2 players (1v1) or 4 players (2v2)
    explicit BriscolaEngine(int playerCount = 2);

    void startNewGame() override;
    void update() override; // Runs AI turns and trick cleanup timers
    bool playCard(std::size_t handIndex) override;

    [[nodiscard]] const GameState& getState() const noexcept override { return m_state; }

private:
    void dealInitialCards();
    void resolveCurrentTrick();
    void replenishHands();
    void syncGameState();
    void triggerAITurn();

    int m_playerCount;
    std::vector<Player> m_players;
    Deck m_deck;
    std::unique_ptr<BriscolaRules> m_rules;
    std::optional<Card> m_trumpCard;

    GameState m_state;
    std::vector<PlayedCard> m_tableCards;

    int m_activeTurnIndex{0};
    int m_trickLeaderIndex{0};
    int m_humanPlayerIndex{0}; // Typically player 0

    float m_trickResolveTimer{0.0f};
};