#pragma once

#include <vector>
#include <optional>
#include <map>
#include <string>
#include "model/Card.hpp"

enum class GamePhase {
    NotStarted,
    PlayingTurn,
    TrickResolving, // Brief pause so players can see who won the trick
    RoundOver,
    GameOver
};

struct PlayerInfo {
    int id;
    std::string name;
    int teamId;
    bool isAI;
    std::size_t handSize;
    int capturedCardsCount;
};

// Information about a card currently on the table
struct PlayedCard {
    int playerId;
    Card card;
};

struct GameState {
    GamePhase phase{GamePhase::NotStarted};

    // Table state
    std::vector<PlayedCard> currentTrick; // Cards played in the active turn
    std::optional<Card> trumpCard;        // The face-up Briscola card
    std::size_t deckRemaining{0};

    // Turn tracking
    int currentPlayerIndex{0};
    int leadingPlayerIndex{0}; // Who led the current trick
    std::optional<int> lastTrickWinnerId;

    // Player views
    std::vector<PlayerInfo> players;
    std::vector<Card> humanHand; // Cards visible to the local human player

    // Scores mapped by teamId (or playerId in 1v1)
    std::map<int, int> teamScores;
    std::string statusMessage; // e.g., "Alice won the trick!", "Your turn"
};