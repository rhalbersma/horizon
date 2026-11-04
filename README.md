# Horizon

Horizon (formerly BoomstraDam) is an international draughts (10×10) playing program written by Feike Boomstra. This fork updates it to build and run on a current Linux system. The original instructions are kept in `README.txt`.

The engine is written in C++ and runs as a separate process, with several threads. The graphical interface is written in Python with Tkinter; it starts the engine and talks to it over a local TCP connection (port 27013).

## Requirements

- Linux, 64-bit
- g++ and make
- Python 3 with Tkinter
- Pillow

## Building the engine

```
cd Horizon
make
```

This produces `Horizon/horizon`. The hash table holds 2^`HASH_BITS` entries of 40 bytes; the default, 22, uses about 160 MB. The original program used 26 (2.5 GB):

```
make clean && make HASH_BITS=26
```

## Running

```
./StartHorizon.sh
```

`StartHorizonTournament.sh` starts the tournament version of the interface.

Data files (images, opening book, PDN files, endgame database) are looked up from the repository root. Set `HORIZON_HOME` to use another directory.

An endgame database in DAM 2.2 format can be placed in `Endgame/Endgame/cpr/`. It is not included; the engine plays without it.

## Playing a game

1. Click **setup** to put the pieces in the starting position.
2. Choose the players: the left column of radio buttons is for White, the right column for Black (computer, human player, remote player, and the side on move).
3. Select **play**.
4. Move a piece by clicking it, then its destination square.

Press Escape or select **pieces set-up** to stop the game.

## Changes from the original

- Plain makefile instead of the Eclipse build files.
- Fixes for current compilers (missing return values, `register`).
- No hard-coded `/home/feike` paths.
- Hash table size set at build time.
- Interface ported to Python 3.

The research tools (CEM weight tuning, LiveDraughts, Swiss tournament ratings) are still in Python 2 and have not been tested.

## License

The engine is released under the GPLv3 and the interface under the Boost Software License 1.0; see `Licenses/` and the headers of the source files.
