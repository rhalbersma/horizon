This archive contains the Horizon (formerly BoomstraDam) 10x10 draughts playing program and source code.
It was written by Feike Boomstra, who died in 2010.

To use Horizon you need a 64-bit Intel/AMD Linux system, e.g. Ubuntu 10.04 x86_64.

The code in its present state uses hard-coded paths, which makes it necessary to create a new user 'feike' on your system, and unpack this archive in home directory /home/feike.

The draughts engine is written in C++ and runs as a separate process with multiple threads to take advantage of multi-core machines.
The engine needs plenty of RAM, 4GB is highly recommended.

The GUI uses Python, you will need to install package python-tk.

Endgame database files in DAM 2.2 format may be placed in the Endgame/Endgame/cpr directory.

About licenses: see the individual source files for license information.
The GUI is released under the Boost license.
The engine executable is released under the GPLv3.

Enjoy!
