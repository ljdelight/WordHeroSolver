# WordHeroSolver
This is a simple recursive solver for the Android game WordHero. The game is a 4x4 grid of letters, like Boggle, where the player must find words within the grid.

This solution loads the dictionary into a DAWG and does not continue recursion when an invalid prefix is found. So this is fairly efficient.  

## How to run and use the solver
To compile the code just use the Makefile: ```$ make```. The Makefile will read the word list and compress it into a binary form.

To run the solver, call ```$ ./solver ```. Note that the binary requires a file called 'dictionary.dat' to be in the same directory; dictionary.dat is the binary representation of the DAWG that is generated when the code is compiled.

## DAWG -- Blitzkrieg Attack Algorithm
I use the DAWG implementation of JohnPaul Adamovsky (big thank you for the speed). For original source files and the DAWG implementation: http://www.pathcom.com/~vadco/dawg.html
