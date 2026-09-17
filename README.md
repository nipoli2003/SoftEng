# SoftEng-group-project

=== WHAT TO RUN WHEN (DONT PANIC CHEATSHEET) ===

Remember: this is C++, you gotta compile your stuff after you change it or nothing happens!!

--- 1. NORMAL EVERYDAY WORK (you just edited some code) ---
Don't rerun cmake, just do the fast build:

[Mac / Linux]
cmake --build build && ./build/card_game

[Windows (PowerShell)]
cmake --build build --config Debug ; .\build\Debug\card_game.exe
(or .\build\card_game.exe if you're on MinGW)

[VS Code shortcuts (easiest)]
- F7 = compile only
- F5 = compile + run with debugger
- Or just click the "Play" button at the bottom bar

--- 2. YOU ADDED A NEW .cpp FILE or TOUCHED CMakeLists.txt ---
CMake needs to see the new files, so run configure again:

[Mac / Linux]
cmake -B build -S .
cmake --build build
./build/card_game

[Windows]
cmake -B build -S .
cmake --build build --config Debug
.\build\Debug\card_game.exe

[VS Code]
Ctrl+Shift+P (or Cmd+Shift+P on Mac) -> search "CMake: Configure"

--- 3. EVERYTHING IS BROKEN / WEIRD LINK ERRORS / HELP ---
Nuke the build folder and let it start from scratch:

[Mac / Linux]
rm -rf build
cmake -B build -S .
cmake --build build
./build/card_game

[Windows (PowerShell)]
rm -r -fo build
cmake -B build -S .
cmake --build build --config Debug
.\build\Debug\card_game.exe

[VS Code]
Ctrl+Shift+P (or Cmd+Shift+P) -> "CMake: Delete Cache and Reconfigure"

Here is the general structure we could follow for the project.

card-game-suite/
├── CMakeLists.txt
├── README.md
│
├── include/                          # Public header files
│   ├── model/                        # BACKEND: Core domain & rules
│   │   ├── Card.hpp
│   │   ├── Deck.hpp
│   │   ├── Hand.hpp
│   │   ├── Player.hpp
│   │   ├── GameRules.hpp             # Strategy interface
│   │   ├── BriscolaRules.hpp         # Concrete strategy 1
│   │   └── ScopaRules.hpp            # Concrete strategy 2
│   │
│   ├── controller/                   # GLUE: Turn orchestration & events
│   │   ├── GameEngine.hpp            # Central state & loop manager
│   │   ├── GameObserver.hpp          # Observer interface (event publisher)
│   │   └── GameState.hpp             # Struct/Enums representing public snapshot
│   │
│   └── view/                         # FRONTEND: UI rendering & input capture
│       ├── IView.hpp                 # Abstract UI interface
│       ├── ConsoleView.hpp           # CLI implementation (terminal)
│       └── GraphicalView.hpp         # (Optional) SFML/Raylib/SDL GUI
│
├── src/                              # Implementation files (.cpp)
│   ├── main.cpp                      # Application entry point
│   │
│   ├── model/
│   │   ├── Card.cpp
│   │   ├── Deck.cpp
│   │   ├── Hand.cpp
│   │   ├── Player.cpp
│   │   ├── BriscolaRules.cpp
│   │   └── ScopaRules.cpp
│   │
│   ├── controller/
│   │   └── GameEngine.cpp
│   │
│   └── view/
│       ├── ConsoleView.cpp
│       └── GraphicalView.cpp         # (If building GUI)
│
├── assets/                           # Optional visual assets (if GUI)
│   └── cards/                        # SVGs / PNGs for Italian card deck
│
└── tests/                            # Unit tests (Catch2 or GoogleTest)
    ├── CMakeLists.txt
    ├── test_deck.cpp
    ├── test_briscola_rules.cpp
    └── test_scopa_rules.cpp