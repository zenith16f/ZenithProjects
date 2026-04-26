# Tetris — Terminal Edition

A fully functional Tetris clone built in **C** with **ncurses**, featuring a clean modern aesthetic in the terminal. Developed in three progressive versions, each building on top of the last.

> Reference: [Brennan Colberg's Tetris tutorial](https://github.com/brenns10/tetris)

---

## 👤 Author

**Fernando** · [@zenith16f](https://github.com/zenith16f)

---

## 🏗️ Architecture

The project follows a clean separation of concerns:

```
tetris/terminal/
├── main.c       # ncurses UI and input handling
├── tetris.c     # Core game logic
├── tetris.h     # Shared types and declarations
└── Makefile
```

Game logic and rendering are completely decoupled — `tetris.c` knows nothing about ncurses.

---

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

---

## ✅ What's done so far

### Version 1.5

- Tiling Manager Problem solved
- Blocked the mouse when playing
- Improved Scoring system
- Arra-Based Gravity
- Hold piece panel

---

## 🚀 Build & Run

bash

```bash
# Install ncurses if needed
sudo apt install libncurses5-dev

# Run
cd Tetris
make run

```

---

## Controls

| Key     | Action            |
| ------- | ----------------- |
| `←` `→` | Move left / right |
| `a` `d` | Move left / right |
| `↓`     | Soft drop         |
| `s`     | Soft drop         |
| `↑`     | Rotate            |
| `w`     | Rotate            |
| `c`     | Hold              |
| `SPC`   | Hard Drop         |
| `q`     | Quit              |

---

## 📜 License

MIT
