# WordHeroSolver
This is a simple recursive solver for the Android game WordHero. 

This solution loads the dictionary into a DAWG and does not continue recursion when an invalid prefix is found. 

## How to run and use the solver
To compile the code just use the Makefile: ```$ make```. The Makefile will read the word list and compress it into a binary form.

To run the solver, call ```$ ./solver <dictionary.dat>```. The dictionary.dat is the binary representation of the DAWG that is generated when the code is compiled. Using a binary file greatly reduces load times. 

## DAWG -- Blitzkrieg Attack Algorithm
I use the DAWG implementation of JohnPaul Adamovsky (big thank you for the speed). For original source files and the DAWG implementation: http://www.pathcom.com/~vadco/dawg.html
