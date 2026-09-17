# 🃏 SoftEng-group-project

Welcome to the repo! We are building an Italian card game suite (starting with **Briscola**, then **Scopa**).

> **Friendly reminder:** This is C++, so your computer doesn't automatically know you edited a file. You have to *compile* your code or literally nothing will change! 💀

---

## 🚀 Build Cheatsheet

### 1. Normal everyday work *(you just edited some code)*

Don't rerun CMake — just do the fast incremental build:

**Mac / Linux**
```bash
cmake --build build && ./build/card_game
```

**Windows (PowerShell)**
```powershell
cmake --build build --config Debug ; .\build\Debug\card_game.exe
# or .\build\card_game.exe if you use MinGW
```

**VS Code shortcuts (easiest)**
| Key | Action |
|-----|--------|
| `F7` | Compile only |
| `F5` | Compile + run with debugger |
| ▶️ | Click the "Play" button in the bottom bar |

### 2. You added a new `.cpp` file or touched `CMakeLists.txt`

CMake needs to see the new files, so run configure again:

**Mac / Linux**
```bash
cmake -B build -S .
cmake --build build
./build/card_game
```

**Windows**
```powershell
cmake -B build -S .
cmake --build build --config Debug
.\build\Debug\card_game.exe
```

**VS Code**
```
Ctrl+Shift+P (Cmd+Shift+P on Mac) → "CMake: Configure"
```

### 3. Everything is broken / weird link errors / help 🔥

Nuke the build folder and let it start fresh from zero:

**Mac / Linux**
```bash
rm -rf build
cmake -B build -S .
cmake --build build
./build/card_game
```

**Windows (PowerShell)**
```powershell
rm -r -fo build
cmake -B build -S .
cmake --build build --config Debug
.\build\Debug\card_game.exe
```

**VS Code**
```
Ctrl+Shift+P (Cmd+Shift+P) → "CMake: Delete Cache and Reconfigure"
```

### 4. Test the implementations

```bash
ctest --test-dir build --output-on-failure
```

---

## 📁 Project Structure

```
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
```

