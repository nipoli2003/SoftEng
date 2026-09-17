#include "view/GraphicalView.hpp"
#include <string>
#include <iomanip>
#include <sstream>

GraphicalView::GraphicalView(GameEngine& engine)
    : m_engine(engine) {}

void GraphicalView::init() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);
    InitWindow(m_windowedWidth, m_windowedHeight, "Card Game Suite");
    SetWindowMinSize(960, 540);
    SetTargetFPS(60);

    loadCardTextures(); // Load textures once GPU context is ready
}

bool GraphicalView::shouldClose() const {
    return WindowShouldClose() || m_shouldExit;
}

void GraphicalView::close() {
    unloadCardTextures(); // Free GPU VRAM
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

void GraphicalView::drawButton(Rectangle bounds, const char* text, bool hovered) {
    Color bg = hovered ? Color{45, 110, 80, 255} : Color{25, 65, 45, 255};
    DrawRectangleRec(bounds, bg);
    DrawRectangleLinesEx(bounds, 2, hovered ? GOLD : RAYWHITE);

    int fontSize = 20;
    int textW = MeasureText(text, fontSize);
    DrawText(text, bounds.x + (bounds.width - textW) / 2, bounds.y + (bounds.height - fontSize) / 2, fontSize, RAYWHITE);
}

void GraphicalView::render(const GameState& state) {
    handleFullscreenToggle();

    BeginDrawing();
    ClearBackground(GetColor(0x1B4D3EFF)); // Table-felt green

    switch (m_currentScene) {
        case AppScene::MainMenu:
            renderMainMenu();
            break;
        case AppScene::LobbyBrowser:
            renderLobbyBrowser();
            break;
        case AppScene::PrivateLobbyWaiting:
            renderPrivateLobby();
            break;
        case AppScene::InGame:
            renderGame(state);
            break;
        default:
            break;
    }

    EndDrawing();
}

void GraphicalView::renderMainMenu() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    const char* title = "CARD GAME SUITE";
    int titleW = MeasureText(title, 40);
    DrawText(title, (sw - titleW) / 2, sh / 4, 40, GOLD);

    Vector2 mousePos = GetMousePosition();
    float btnW = 280.0f;
    float btnH = 50.0f;
    float startY = sh / 2.0f - 40.0f;

    // 1. Play Solo (Offline vs CPU)
    Rectangle btnSolo{(sw - btnW) / 2.0f, startY, btnW, btnH};
    bool hoverSolo = CheckCollisionPointRec(mousePos, btnSolo);
    drawButton(btnSolo, "Play Solo (vs CPU)", hoverSolo);
    if (hoverSolo && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_engine.startNewGame();
        m_currentScene = AppScene::InGame;
    }

    // 2. Private Lobby (Online Multiplayer)
    Rectangle btnLobby{(sw - btnW) / 2.0f, startY + 65.0f, btnW, btnH};
    bool hoverLobby = CheckCollisionPointRec(mousePos, btnLobby);
    drawButton(btnLobby, "Private Lobby (Online)", hoverLobby);
    if (hoverLobby && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_currentScene = AppScene::PrivateLobbyWaiting;
    }

    // 3. Exit
    Rectangle btnExit{(sw - btnW) / 2.0f, startY + 130.0f, btnW, btnH};
    bool hoverExit = CheckCollisionPointRec(mousePos, btnExit);
    drawButton(btnExit, "Exit", hoverExit);
    if (hoverExit && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_shouldExit = true;
    }
}

void GraphicalView::renderPrivateLobby() {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    DrawText("PRIVATE LOBBY", (sw - MeasureText("PRIVATE LOBBY", 32)) / 2, 80, 32, GOLD);

    std::string codeStr = "Room Code: " + m_lobbyCode;
    DrawText(codeStr.c_str(), (sw - MeasureText(codeStr.c_str(), 24)) / 2, 130, 24, RAYWHITE);
    DrawText("Share this code with your friend to connect.", 
             (sw - MeasureText("Share this code with your friend to connect.", 16)) / 2, 165, 16, LIGHTGRAY);

    Rectangle boxRec{(sw - 400.0f) / 2.0f, 210.0f, 400.0f, 200.0f};
    DrawRectangleRec(boxRec, Color{15, 40, 30, 220});
    DrawRectangleLinesEx(boxRec, 2, DARKGRAY);

    DrawText("Players Connected (1/2):", boxRec.x + 20, boxRec.y + 15, 18, GOLD);
    for (std::size_t i = 0; i < m_lobbyPlayers.size(); ++i) {
        DrawText(TextFormat("%zu. %s", i + 1, m_lobbyPlayers[i].c_str()), 
                 boxRec.x + 30, boxRec.y + 50 + (i * 30), 18, RAYWHITE);
    }
    DrawText("Waiting for opponent to join...", boxRec.x + 30, boxRec.y + 140, 16, GRAY);

    Vector2 mousePos = GetMousePosition();
    Rectangle btnStart{(sw - 200.0f) / 2.0f - 110.0f, 440.0f, 200.0f, 45.0f};
    bool hoverStart = CheckCollisionPointRec(mousePos, btnStart);
    drawButton(btnStart, "Start Game", hoverStart);
    if (hoverStart && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_engine.startNewGame();
        m_currentScene = AppScene::InGame;
    }

    Rectangle btnBack{(sw - 200.0f) / 2.0f + 110.0f, 440.0f, 200.0f, 45.0f};
    bool hoverBack = CheckCollisionPointRec(mousePos, btnBack);
    drawButton(btnBack, "Back to Menu", hoverBack);
    if (hoverBack && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_currentScene = AppScene::MainMenu;
    }
}

void GraphicalView::renderLobbyBrowser() {
    // Reserved for public server browser
}

void GraphicalView::renderGame(const GameState& state) {
    if (state.phase == GamePhase::PlayingTurn && state.currentPlayerIndex == 0) {
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

    drawTable(state);
    drawPlayerHand(state);
    drawHUD(state);

    Rectangle btnLeave{15.0f, 15.0f, 90.0f, 30.0f};
    bool hoverLeave = CheckCollisionPointRec(GetMousePosition(), btnLeave);
    drawButton(btnLeave, "< Menu", hoverLeave);
    if (hoverLeave && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        m_currentScene = AppScene::MainMenu;
    }
}

std::string GraphicalView::getCardFilename(const Card& card) const {
    // 1. Determine suit offset and name
    int suitOffset = 0;
    std::string suitName;

    switch (card.suit()) {
        case Suit::Spade_Spades:
            suitOffset = 0;
            suitName = "Spade";
            break;
        case Suit::Bastoni_Clubs:
            suitOffset = 10;
            suitName = "Bastoni";
            break;
        case Suit::Coppe_Hearts:
            suitOffset = 20;
            suitName = "Coppe";
            break;
        case Suit::Denari_Diamonds:
            suitOffset = 30;
            suitName = "Denari";
            break;
        default:
            return "unknown";
    }

    // 2. Determine rank index (1-10) and rank string
    int rankIndex = 0;
    std::string rankName;

    switch (card.rank()) {
        case Rank::Ace:
            rankIndex = 1;
            rankName = "Asso";
            break;
        case Rank::Jack_Fante:
            rankIndex = 8;
            rankName = "Fante";
            break;
        case Rank::Queen_Regina: // Used as Cavallo
            rankIndex = 9;
            rankName = "Cavallo";
            break;
        case Rank::King_Re:
            rankIndex = 10;
            rankName = "Re";
            break;
        default:
            rankIndex = static_cast<int>(card.rank());
            rankName = std::to_string(rankIndex);
            break;
    }

    // 3. Format into "XX_Rank_di_Suit" (e.g. "01_Asso_di_Spade", "18_Fante_di_Bastoni")
    int cardIndex = suitOffset + rankIndex;

    std::ostringstream ss;
    ss << std::setw(2) << std::setfill('0') << cardIndex 
       << "_" << rankName << "_di_" << suitName;

    return ss.str();
}

void GraphicalView::loadCardTextures() {
    // Card back
    m_cardBackTexture = LoadTexture("assets/cards/41_card_back.png");
    if (m_cardBackTexture.id != 0) {
        SetTextureFilter(m_cardBackTexture, TEXTURE_FILTER_BILINEAR);
    }

    // All 40 Italian cards
    constexpr std::array<Suit, 4> suits = {
        Suit::Spade_Spades, Suit::Bastoni_Clubs, Suit::Coppe_Hearts, Suit::Denari_Diamonds
    };
    constexpr std::array<Rank, 10> ranks = {
        Rank::Ace, Rank::Two, Rank::Three, Rank::Four, Rank::Five,
        Rank::Six, Rank::Seven, Rank::Jack_Fante, Rank::Queen_Regina, Rank::King_Re
    };

    for (auto suit : suits) {
        for (auto rank : ranks) {
            Card card(rank, suit, 0);
            std::string key = getCardFilename(card);
            std::string path = "assets/cards/" + key + ".png";

            Texture2D tex = LoadTexture(path.c_str());
            if (tex.id != 0) {
                SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
                m_cardTextures[key] = tex;
            } else {
                TraceLog(LOG_WARNING, "FAILED TO LOAD TEXTURE: %s", path.c_str());
            }
        }
    }
    m_texturesLoaded = true;
}

void GraphicalView::unloadCardTextures() {
    if (!m_texturesLoaded) return;

    UnloadTexture(m_cardBackTexture);
    for (auto& [_, tex] : m_cardTextures) {
        UnloadTexture(tex);
    }
    m_cardTextures.clear();
    m_texturesLoaded = false;
}

void GraphicalView::drawCard(const Card& card, Rectangle dest, bool faceUp) {
    if (!faceUp) {
        if (m_cardBackTexture.id != 0) {
            Rectangle srcRect{0.0f, 0.0f, static_cast<float>(m_cardBackTexture.width), static_cast<float>(m_cardBackTexture.height)};
            DrawTexturePro(m_cardBackTexture, srcRect, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
        } else {
            // Fallback placeholder
            DrawRectangleRec(dest, Color{70, 30, 30, 255});
            DrawRectangleLinesEx(dest, 2, RAYWHITE);
        }
        return;
    }

    std::string key = getCardFilename(card);
    auto it = m_cardTextures.find(key);

    if (it != m_cardTextures.end() && it->second.id != 0) {
        Texture2D tex = it->second;
        Rectangle srcRect{0.0f, 0.0f, static_cast<float>(tex.width), static_cast<float>(tex.height)};
        
        // Stretches/scales sprite to fit dest rectangle cleanly
        DrawTexturePro(tex, srcRect, dest, Vector2{0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        // Fallback placeholder if image file is missing
        DrawRectangleRec(dest, RAYWHITE);
        DrawRectangleLinesEx(dest, 2, DARKGRAY);
        DrawText(key.c_str(), dest.x + 5, dest.y + dest.height / 2 - 10, 14, BLACK);
    }
}

void GraphicalView::drawTable(const GameState& state) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    float deckX = 50.0f;
    float deckY = sh / 2.0f - 60.0f;

    if (state.trumpCard) {
        Rectangle trumpRect{deckX + 30.0f, deckY + 20.0f, 120.0f, 80.0f};
        drawCard(*state.trumpCard, trumpRect, true);

        if (state.deckRemaining > 1) {
            Rectangle deckRect{deckX, deckY, 80.0f, 120.0f};
            drawCard(Card(Rank::Ace, Suit::Bastoni_Clubs, 0), deckRect, false);
        }
    }

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

        if (CheckCollisionPointRec(GetMousePosition(), bounds) && state.currentPlayerIndex == 0) {
            bounds.y -= 12.0f;
        }

        m_handCardBounds.push_back(bounds);
        drawCard(state.humanHand[i], bounds, true);
    }
}

void GraphicalView::drawHUD(const GameState& state) {
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    if (state.phase != GamePhase::GameOver) {
        DrawText(state.statusMessage.c_str(), 
                 (sw - MeasureText(state.statusMessage.c_str(), 20)) / 2, 
                 25, 20, RAYWHITE);
    }

    std::string deckInfo = "Deck: " + std::to_string(state.deckRemaining);
    DrawText(deckInfo.c_str(), 40, 30, 18, LIGHTGRAY);

    int yOffset = 30;
    for (const auto& [teamId, score] : state.teamScores) {
        std::string scoreStr = "Team " + std::to_string(teamId) + ": " + std::to_string(score) + " pts";
        DrawText(scoreStr.c_str(), sw - 220, yOffset, 18, RAYWHITE);
        yOffset += 24;
    }

    if (state.phase == GamePhase::GameOver) {
        DrawRectangle(0, 0, sw, sh, Color{0, 0, 0, 160});

        int bannerW = 600;
        int bannerH = 160;
        int bannerX = (sw - bannerW) / 2;
        int bannerY = (sh - bannerH) / 2;

        DrawRectangle(bannerX, bannerY, bannerW, bannerH, Color{25, 25, 25, 240});
        DrawRectangleLinesEx(Rectangle{static_cast<float>(bannerX), static_cast<float>(bannerY), static_cast<float>(bannerW), static_cast<float>(bannerH)}, 3, GOLD);

        const char* msg = state.statusMessage.c_str();
        int fontSize = 36;
        int textW = MeasureText(msg, fontSize);

        Color textColor = RAYWHITE;
        if (state.statusMessage.rfind("YOU WIN", 0) == 0) textColor = GREEN;
        else if (state.statusMessage.rfind("YOU LOSE", 0) == 0) textColor = RED;
        else textColor = YELLOW;

        DrawText(msg, (sw - textW) / 2, bannerY + 45, fontSize, textColor);

        const char* subMsg = "Thanks for playing!";
        DrawText(subMsg, (sw - MeasureText(subMsg, 20)) / 2, bannerY + 100, 20, LIGHTGRAY);
    }
}