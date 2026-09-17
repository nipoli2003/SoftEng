#include "controller/BriscolaEngine.hpp"
#include <stdexcept>

BriscolaEngine::BriscolaEngine(int playerCount)
    : m_playerCount(playerCount) {
    if (playerCount != 2 && playerCount != 4) {
        throw std::invalid_argument("Briscola supports only 2 or 4 players.");
    }
}

void BriscolaEngine::startNewGame() {
    m_players.clear();
    m_tableCards.clear();

    // Player 0 is Human, others are AI
    for (int i = 0; i < m_playerCount; ++i) {
        int teamId = (m_playerCount == 4) ? (i % 2) : i;
        bool isAI = (i != m_humanPlayerIndex);
        std::string name = (i == m_humanPlayerIndex) ? "Player" : ("CPU " + std::to_string(i));
        m_players.emplace_back(i, name, isAI, teamId);
    }

    m_deck = DeckFactory::createItalian40();
    m_deck.shuffle();

    // Draw the trump card (Briscola)
    m_trumpCard = m_deck.draw();
    if (m_trumpCard) {
        m_rules = std::make_unique<BriscolaRules>(m_trumpCard->suit());
    }

    dealInitialCards();

    m_activeTurnIndex = 0;
    m_trickLeaderIndex = 0;
    m_state.phase = GamePhase::PlayingTurn;
    m_state.statusMessage = "Game started. Your turn!";

    syncGameState();
}

void BriscolaEngine::dealInitialCards() {
    // Standard Briscola deals 3 cards to each player
    for (int round = 0; round < 3; ++round) {
        for (auto& player : m_players) {
            auto card = m_deck.draw();
            if (card) player.hand().addCard(*card);
        }
    }
}

bool BriscolaEngine::playCard(std::size_t handIndex) {
    if (m_state.phase != GamePhase::PlayingTurn) return false;

    Player& current = m_players[m_activeTurnIndex];
    auto played = current.hand().playCard(handIndex);
    if (!played) return false;

    m_tableCards.push_back({current.id(), *played});

    // Check if full trick completed
    if (m_tableCards.size() == static_cast<std::size_t>(m_playerCount)) {
        resolveCurrentTrick();
    } else {
        m_activeTurnIndex = (m_activeTurnIndex + 1) % m_playerCount;
    }

    syncGameState();
    return true;
}

void BriscolaEngine::update() {
    if (m_state.phase != GamePhase::PlayingTurn) return;

    if (m_players[m_activeTurnIndex].isAI()) {
        triggerAITurn();
    }
}

void BriscolaEngine::triggerAITurn() {
    // Baseline AI: plays the first available card
    playCard(0);
}

void BriscolaEngine::resolveCurrentTrick() {
    std::vector<Card> trickCards;
    for (const auto& item : m_tableCards) {
        trickCards.push_back(item.card);
    }

    std::size_t winIndex = m_rules->evaluateTrick(trickCards);
    int winnerId = m_tableCards[winIndex].playerId;

    // Distribute captured cards to the trick winner
    for (const auto& item : m_tableCards) {
        m_players[winnerId].captureCard(item.card);
    }

    m_state.lastTrickWinnerId = winnerId;
    m_state.statusMessage = m_players[winnerId].name() + " won the trick!";

    // Winner leads next round
    m_activeTurnIndex = winnerId;
    m_trickLeaderIndex = winnerId;
    m_tableCards.clear();

    replenishHands();

    // Check game over condition
    bool allEmpty = true;
    for (const auto& p : m_players) {
        if (!p.hand().empty()) {
            allEmpty = false;
            break;
        }
    }

    if (allEmpty && m_deck.empty() && !m_trumpCard.has_value()) {
        m_state.phase = GamePhase::GameOver;
        m_state.statusMessage = "Game Over!";
    }
}

void BriscolaEngine::replenishHands() {
    // Each player draws 1 card, starting with trick winner
    for (int i = 0; i < m_playerCount; ++i) {
        int targetIdx = (m_trickLeaderIndex + i) % m_playerCount;

        auto card = m_deck.draw();
        if (card) {
            m_players[targetIdx].hand().addCard(*card);
        } else if (m_trumpCard) {
            // Last remaining card to draw is the revealed trump card
            m_players[targetIdx].hand().addCard(*m_trumpCard);
            m_trumpCard.reset();
        }
    }
}

void BriscolaEngine::syncGameState() {
    m_state.currentTrick = m_tableCards;
    m_state.trumpCard = m_trumpCard;
    m_state.deckRemaining = m_deck.remainingCards() + (m_trumpCard ? 1 : 0);
    m_state.currentPlayerIndex = m_activeTurnIndex;
    m_state.leadingPlayerIndex = m_trickLeaderIndex;

    m_state.players.clear();
    for (const auto& p : m_players) {
        m_state.players.push_back({
            p.id(),
            p.name(),
            p.teamId(),
            p.isAI(),
            p.hand().size(),
            static_cast<int>(p.capturedCards().size())
        });
    }

    // Pass visible cards to the local human
    m_state.humanHand = m_players[m_humanPlayerIndex].hand().cards();

    if (m_rules) {
        m_state.teamScores = m_rules->calculateTeamScores(m_players);
    }
}