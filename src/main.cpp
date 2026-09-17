#include "controller/BriscolaEngine.hpp"
#include "view/GraphicalView.hpp"
#include <iostream>

int main() {
    std::cout << "Booting Card Game Suite..." << std::endl;

    // 2-player game (1v1). Pass 4 for 2v2.
    BriscolaEngine engine(2);
    engine.startNewGame();

    GraphicalView view(engine);
    view.init();

    while (!view.shouldClose()) {
        engine.update();
        view.render(engine.getState());
    }

    view.close();
    return 0;
}