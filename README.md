# Memory Game

A terminal-based memory matching game built in C using the `ncurses` library. Match pairs of cards by flipping them two at a time. Play solo, against a friend, or versus a simple computer bot. Customize the board size and visual themes.

## Features

- Three game modes:  
  - Player vs Player  
  - Player vs Computer  
  - Solo Practice
- Adjustable board size
- Theme customization (persistent across restarts)
- `ncurses`-based UI for a smooth terminal experience
- Keyboard controls (WASD / arrow keys + Space/Enter)


## Getting Started

### Prerequisites

Linux is the recommended platform. Running the game on Windows is possible, but not officially supported.

Install `ncurses` on Debian-based systems:

```bash
sudo apt install libncurses5-dev
```

### Build Instructions

Compile the game using `gcc`:

```bash
gcc -o memory-game main.c -lncurses
```

If you're using VS Code, make sure to include the provided `.vscode` folder with `launch.json`, `settings.json`, and `tasks.json` (modifications might be needed).

### Run the Game

```bash
./memory-game
```

## Controls

| Action    | Keys               |
|-----------|--------------------|
| Move      | WASD or Arrow keys |
| Select    | Space or Enter     |

## Settings

Game settings, including selected themes, are stored in a file called:

```
settings for memory game.txt
```

This allows you to preserve your preferences between sessions.

## File Structure

```
├── main.c
├── settings for memory game.txt
└── .vscode/
    ├── launch.json
    ├── settings.json
    └── tasks.json
```

## Known Issues

- Windows compatibility requires manual configuration




## Author

**Shteinberg Andrei**  
