#pragma once

#include "GameState.hpp"

class GameEngine {
public:
    virtual ~GameEngine() = default;

    virtual void startNewGame() = 0;
    virtual void update() = 0;
    virtual bool playCard(std::size_t handIndex) = 0;

    [[nodiscard]] virtual const GameState& getState() const noexcept = 0;
};