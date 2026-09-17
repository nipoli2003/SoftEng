#pragma once

#include "controller/GameState.hpp"

class IView {
public:
    virtual ~IView() = default;

    virtual void init() = 0;
    virtual void render(const GameState& state) = 0;
    virtual bool shouldClose() const = 0;
    virtual void close() = 0;
};