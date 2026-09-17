#include "controller/BriscolaEngine.hpp"
#include "view/GraphicalView.hpp"
#include <iostream>

int main() {
    std::cout << "Booting Card Game Suite..." << std::endl;

    BriscolaEngine engine(2);
    GraphicalView view(engine);

    view.init();

    while (!view.shouldClose()) {
        engine.update();
        view.render(engine.getState());
    }

    view.close();
    return 0;
}