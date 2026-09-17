# 🃏 SoftEng-group-project

Welcome to the repo! We are building an Italian card game suite (starting with Briscola, then Scopa).

> Friendly reminder: This is C++, so your computer doesn't automatically know you edited a file. You have to compile your code or literally nothing will change! 💀

=======================================================
=== THE "DON'T PANIC" BUILD CHEATSHEET              ===
=======================================================

--- 1. NORMAL EVERYDAY WORK (you just edited some code) ---
Don't rerun cmake, just do the fast incremental build:

[Mac / Linux]
cmake --build build && ./build/card_game

[Windows (PowerShell)]
cmake --build build --config Debug ; .\build\Debug\card_game.exe
(or .\build\card_game.exe if you use MinGW)

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
Nuke the build folder and let it start fresh from zero:

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

--- 4. TEST THE IMPLEMENTATIONS ---
compile: ctest --test-dir build --output-on-failure
to test the implementation of the game.


=======================================================
=== PROJECT STRUCTURE                               ===
=======================================================

SoftEng/
├── CMakeLists.txt
├── README.md
│
├── include/                          # Headers (.hpp go here)
│   ├── model/                        # BACKEND: Card, Deck, and Rules logic
│   │   ├── Card.hpp
│   │   ├── Deck.hpp
│   │   ├── Hand.hpp
│   │   ├── Player.hpp
│   │   ├── GameRules.hpp             # Shared rules interface
│   │   ├── BriscolaRules.hpp         # Briscola mechanics & scoring
│   │   └── ScopaRules.hpp            # Scopa mechanics & captures
│   │
│   ├── controller/                   # GLUE: Turn orchestration & loops
│   │   ├── GameEngine.hpp
│   │   ├── GameObserver.hpp
│   │   └── GameState.hpp
│   │
│   └── view/                         # FRONTEND: Screen stuff & inputs
│       ├── IView.hpp
│       ├── ConsoleView.hpp           # Terminal UI
│       └── GraphicalView.hpp         # Raylib 2D GUI
│
├── src/                              # Actual implementations (.cpp go here)
│   ├── main.cpp                      # Starts the game
│   ├── model/
│   ├── controller/
│   └── view/
│
├── assets/                           # Card textures / SVGs / sprites
│   └── cards/
│
└── tests/                            # Unit tests (let's keep these green!)
    ├── CMakeLists.txt
    ├── test_deck.cpp
    ├── test_briscola_rules.cpp
    └── test_scopa_rules.cpp