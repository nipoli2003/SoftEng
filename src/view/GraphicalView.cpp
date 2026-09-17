#include "view/GraphicalView.hpp"
#include <string>

GraphicalView::GraphicalView(GameEngine& engine)
    : m_engine(engine) {}

void GraphicalView::init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);
    InitWindow(m_windowedWidth, m_windowedHeight, "Card Game Suite - Briscola");
    SetWindowMinSize(960, 540);
    SetTargetFPS(60);
}

bool GraphicalView::shouldClose() const {
    return WindowShouldClose();
}

void GraphicalView::close() {
    CloseWindow();
}

void GraphicalView::handleFullscreenToggle() {
    if (IsKeyPressed(KEY_F) || IsKeyPressed(KEY_F11)) {
        if (IsWindowState(FLAG_WINDOW_MAXIMIZED)) {
            ClearWindowState(FLAG_WINDOW_MAXIMIZED);
            ClearWindowState(FLAG_WINDOW_UNDECORATED);
            SetWindowSize(m_windowedWidth, m_windowedHeight);
        } else {
            m_windowedWidth = GetScreenWidth();
            m_windowedHeight = GetScreenHeight();
            SetWindowState(FLAG_WINDOW_MAXIMIZED | FLAG_WINDOW_UNDECORATED);
        }
    }
}

void GraphicalView::handleInput(const GameState& state) {
    handleFullscreenToggle();

    // Only allow human card clicks when it's our turn
    if (state.phase != GamePhase::PlayingTurn) return;
    if (state.currentPlayerIndex != 0) return; // Player 0 is the local user

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        for (std::size_t i = 0; i < m_handCardBounds.size(); ++i) {
            if (CheckCollisionPointRec(mousePos, m_handCardBounds[i])) {
                m_engine.playCard(i);
                break;
            }
        }
    }
}

void GraphicalView::render(const GameState& state) {
    handleInput(state);

    BeginDrawing();
    ClearBackground(GetColor(0x1B4D3EFF)); // Classic table-felt green

    drawTable(state);
    drawPlayerHand(state);
    drawHUD(state);

    EndDrawing();
}

void GraphicalView::drawCard(const Card& card, Rectangle dest, bool faceUp) {
    if (!faceUp) {
        DrawRectangleRec(dest, Color{70, 30, 30, 255});
        DrawRectangleLinesEx(dest, 2, RAYWHITE);
        DrawText("CARD", dest.x + 10, dest.y + dest.height / 2 - 8, 14, RAYWHITE);
        return;
    }

    DrawRectangleRec(dest, RAYWHITE);
    DrawRectangleLinesEx(dest, 2, DARKGRAY);

    std::string rankStr = std::to_string(static_cast<int>(card.rank()));
    if (card.rank() == Rank::Ace) rankStr = "A";
    else if (card.rank() == Rank::Jack_Fante) rankStr = "J";
    else if (card.rank() == Rank::Knight_Cavallo) rankStr = "C";
    else if (card.rank() == Rank::King_Re) rankStr = "K";

    std::string suitStr;
    Color suitColor = BLACK;
    switch (card.suit()) {
        case Suit::Bastoni_Clubs:   suitStr = "Bastoni"; suitColor = DARKGREEN; break;
        case Suit::Coppe_Hearts:    suitStr = "Coppe";   suitColor = RED; break;
        case Suit::Denari_Diamonds: suitStr = "Denari";  suitColor = GOLD; break;
        case Suit::Spade_Spades:    suitStr = "Spade";   suitColor = BLACK; break;
        default: suitStr = "?"; break;
    }

    // Helper lambda for thicker/bold text
    auto drawTextThick = [](const char* text, int posX, int posY, int fontSize, Color color) {
        DrawText(text, posX + 1, posY, fontSize, color);
        DrawText(text, posX, posY + 1, fontSize, color);
        DrawText(text, posX + 1, posY + 1, fontSize, color);
        DrawText(text, posX, posY, fontSize, color);
    };

    drawTextThick(rankStr.c_str(), dest.x + 10, dest.y + 10, 24, suitColor);
    drawTextThick(suitStr.c_str(), dest.x + 10, dest.y + dest.height - 28, 18, suitColor);
}

void GraphicalView::drawTable(const GameState& state) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // 1. Deck and Briscola display
    float deckX = 50.0f;
    float deckY = sh / 2.0f - 60.0f;

    if (state.trumpCard) {
        // Draw the Briscola card horizontally beneath the deck, extended to the right
        Rectangle trumpRect{deckX + 30.0f, deckY + 20.0f, 120.0f, 80.0f};
        drawCard(*state.trumpCard, trumpRect, true);

        // Draw deck vertically over the left half of the trump card
        if (state.deckRemaining > 1) {
            Rectangle deckRect{deckX, deckY, 80.0f, 120.0f};
            drawCard(Card(Rank::Ace, Suit::Bastoni_Clubs, 0), deckRect, false);
        }
    }

    // 2. Center table: cards played in the current trick
    float startX = sw / 2.0f - (state.currentTrick.size() * 105.0f) / 2.0f;
    float startY = sh / 2.0f - 60.0f;

    for (std::size_t i = 0; i < state.currentTrick.size(); ++i) {
        Rectangle cardRect{startX + i * 105.0f, startY, 80.0f, 120.0f};
        drawCard(state.currentTrick[i].card, cardRect, true);

        std::string playerLabel = (state.currentTrick[i].playerId == 0) ? "You" : "CPU";
        DrawText(playerLabel.c_str(), cardRect.x + 22, cardRect.y - 22, 18, RAYWHITE);
    }
}

void GraphicalView::drawPlayerHand(const GameState& state) {
    m_handCardBounds.clear();

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    float cardW = 90.0f;
    float cardH = 135.0f;
    float spacing = 20.0f;

    float totalW = state.humanHand.size() * cardW + (state.humanHand.size() > 1 ? (state.humanHand.size() - 1) * spacing : 0.0f);
    float startX = (sw - totalW) / 2.0f;
    float startY = sh - cardH - 30.0f;

    for (std::size_t i = 0; i < state.humanHand.size(); ++i) {
        Rectangle bounds{startX + i * (cardW + spacing), startY, cardW, cardH};

        // Subtle lift effect on mouse hover
        if (CheckCollisionPointRec(GetMousePosition(), bounds) && state.currentPlayerIndex == 0) {
            bounds.y -= 12.0f;
        }

        m_handCardBounds.push_back(bounds);
        drawCard(state.humanHand[i], bounds, true);
    }
}

void GraphicalView::drawHUD(const GameState& state) {
    int sw = GetScreenWidth();

    // Top status message
    DrawText(state.statusMessage.c_str(), 
             (sw - MeasureText(state.statusMessage.c_str(), 20)) / 2, 
             25, 20, RAYWHITE);

    // Deck count
    std::string deckInfo = "Deck: " + std::to_string(state.deckRemaining);
    DrawText(deckInfo.c_str(), 40, 30, 18, LIGHTGRAY);

    // Score display
    int yOffset = 30;
    for (const auto& [teamId, score] : state.teamScores) {
        std::string scoreStr = "Team " + std::to_string(teamId) + " Points: " + std::to_string(score);
        DrawText(scoreStr.c_str(), sw - 220, yOffset, 18, RAYWHITE);
        yOffset += 24;
    }
}