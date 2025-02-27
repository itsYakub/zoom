
	 ____   ___     ___    _ __ ___	
	|_  /  / _ \   / _ \  | '_ ` _ \
	 / /  | (_) | | (_) | | | | | | |
	/___|  \___/   \___/  |_| |_| |_|


===============================================================

0. Introduction

'zoom' is a linux utility for X11 Windowing Server.
It magnifies parts of the screen in a simple and elegant manner.

'zoom' is writter for X11 Windowing Server, and uses OpenGL for
rendering. Everything in this project is written in C (c99).

'zoom' takes so much inspiration from 'boomer' made by @tsoding.
Original utility was written in Nim.

===============================================================

1. Setup

For this project we'll need two packages:
- GNU Toolkit / Build Essentials;
- git;

1.1. Get the repository from hosting platform.

$ git clone https://github.com/itsYakub/zoom.git
$ cd zoom/

1.2. Build the project using GNU Make:

$ make all

To install this utility on your machine, use:

$ sudo make install

This command should install 'zoom' at '/usr/local/bin'

To uninstall this utility, type:

$ sudo make uninstall

===============================================================

2. Licence

This project is under the:
	"GNU Lesser General Public Licence" (LGPL).
