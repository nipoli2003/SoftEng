#pragma once

#include "view/IView.hpp"
#include "controller/GameEngine.hpp"
#include "raylib.h"
#include <vector>

class GraphicalView : public IView {
public:
    explicit GraphicalView(GameEngine& engine);
    ~GraphicalView() override = default;

    void init() override;
    void render(const GameState& state) override;
    [[nodiscard]] bool shouldClose() const override;
    void close() override;

private:
    void handleInput(const GameState& state);
    void handleFullscreenToggle();

    // Drawing helpers
    void drawCard(const Card& card, Rectangle dest, bool faceUp = true);
    void drawTable(const GameState& state);
    void drawPlayerHand(const GameState& state);
    void drawHUD(const GameState& state);

    GameEngine& m_engine;
    int m_windowedWidth{1280};
    int m_windowedHeight{720};

    // Clickable hitboxes for human player hand cards
    std::vector<Rectangle> m_handCardBounds;
};