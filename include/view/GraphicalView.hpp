#pragma once

#include "view/IView.hpp"
#include "view/AppScene.hpp"
#include "controller/GameEngine.hpp"
#include "raylib.h"
#include <vector>
#include <string>
#include <map>

class GraphicalView : public IView {
public:
    explicit GraphicalView(GameEngine& engine);
    ~GraphicalView() override = default;

    void init() override;
    void render(const GameState& state) override;
    [[nodiscard]] bool shouldClose() const override;
    void close() override;

private:

    void loadCardTextures();
    void unloadCardTextures();
    [[nodiscard]] std::string getCardFilename(const Card& card) const;

    void handleFullscreenToggle();

    // Scene Renderers & Handlers
    void renderMainMenu();
    void renderLobbyBrowser();
    void renderPrivateLobby();
    void renderGame(const GameState& state);

    // Drawing Helpers
    void drawButton(Rectangle bounds, const char* text, bool hovered);
    void drawCard(const Card& card, Rectangle dest, bool faceUp = true);
    void drawTable(const GameState& state);
    void drawPlayerHand(const GameState& state);
    void drawHUD(const GameState& state);

    std::map<std::string, Texture2D> m_cardTextures;
    Texture2D m_cardBackTexture{};
    bool m_texturesLoaded{false};

    GameEngine& m_engine;
    AppScene m_currentScene{AppScene::MainMenu};
    bool m_shouldExit{false};

    int m_windowedWidth{1280};
    int m_windowedHeight{720};

    // Lobby stub state (ready for server integration)
    std::string m_lobbyCode{"BRIS-4092"};
    std::vector<std::string> m_lobbyPlayers{"HostPlayer (You)"};
    bool m_isHost{true};

    std::vector<Rectangle> m_handCardBounds;
};