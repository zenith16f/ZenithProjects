#  Tetris — Terminal Edition

A fully functional Tetris clone built in **C** with **ncurses**, featuring a clean modern aesthetic in the terminal. Developed in three progressive versions, each building on top of the last.

> Reference: [Brennan Colberg's Tetris tutorial](https://github.com/brennancolberg/tetris)

------

## 👤 Author

**Fernando** · [@zenith16f](https://github.com/zenith16f)

------

## 🏗️ Architecture

The project follows a clean separation of concerns:

```
tetris/
├── main.c       # ncurses UI and input handling
├── tetris.c     # Core game logic
├── tetris.h     # Shared types and declarations
└── Makefile
```

Game logic and rendering are completely decoupled — `tetris.c` knows nothing about ncurses.

------

## 🗺️ Version Roadmap

### ✅ Version 1 — Core Mechanics

> Basic but fully playable Tetris.

- Linear gravity (pieces fall at a fixed interval)
- Simple clockwise rotation (no wall kicks)
- Collision detection against walls, floor, and locked pieces
- Line clearing
- Game over detection
- Modern terminal aesthetic — two-space colored blocks
- Score and next piece panels

### 🔲 Version 2 — Better Feel

> The game starts to feel like real Tetris.

- Array-based gravity (variable fall speed by level)
- Lock delay (piece doesn't lock instantly on landing)
- Soft drop & hard drop
- Ghost piece (shadow showing where the piece will land)
- Improved scoring system

### 🔲 Version 3 — Full Fidelity

> Official Tetris guideline compliance.

- Super Rotation System (SRS) with wall kicks
- Official Tetris scoring (single / double / triple / Tetris)
- Level progression and speed curve
- T-spin detection
- Background music

------

## ✅ What's done so far

### Version 1

-  Project structure with header files and Makefile
-  ncurses initialization and teardown
-  Game board representation
-  Tetromino definitions as coordinate offset arrays
-  Tetromino spawning and randomization
-  Rendering loop (board + active piece + UI panels)
-  Gravity timer system
-  Left / right movement with collision detection
-  Clockwise rotation with collision detection
-  Piece locking on landing
-  Line clearing logic
-  Score tracking
-  Next piece preview panel
-  Game over screen

------

## 🚀 Build & Run

bash

```bash
# Install ncurses if needed
sudo apt install libncurses5-dev

# Run
cd Tetris
make run

```

------

## Controls

| Key     | Action            |
| ------- | ----------------- |
| `←` `→` | Move left / right |
| `↓`     | Soft drop         |
| `↑`     | Rotate            |
| `q`     | Quit              |

------

## 📜 License

MIT	