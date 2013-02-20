/*
 * This program will show all possible words of length 3 or more
 *   given a WordHero 4x4 grid. I use a directed acyclic word graph
 *   implementation found at [1].
 *
 *   - This program takes a _binary_ version of the dictionary for speed.
 *         Call as ./solver <dictionary.dat>
 *   - The binary version is created when 'make' is called; it uses code at [1] to
 *         read in the dictionary and encodes it into the output file.
 *
 *  [1] http://www.pathcom.com/~vadco/dawg.html
 *      Note that I modified the code a bit to eliminate stdout output.
 *
 *  Created on: Dec 18, 2012
 *      Author: ljdelight
 *
 *
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>
#include <fstream>
#include <set>

#include "WordHeroSolver.h"
using namespace std;

int main( int argc, char* argv[] )
{
	//
	// SOLVE A STATIC STRING
	//
//	const char* sample = "HAGOLLBRISEMMETW";
//	int size = strlen(sample);
//	assert( size <= N_ROWS*N_COLS );
//
//	for ( int i = 0; i < size; ++i )
//	{
//		board[i] = Char( sample[i] );
//	}
//	for ( int i = 0; i < N_ROWS; ++i )
//	{
//		for ( int j = 0; j < N_COLS; ++j )
//		{
//			solver( board, N_ROWS, N_COLS, str, 0, MAX_STRING_LENGTH, i, j, dawg );
//		}
//	}
//	auto itr = wordsFound.begin(), end = wordsFound.end();
//	for ( ; itr != end; ++itr )
//	{
//		cout << *itr << '\n';
//	}

	WordHeroSolver solver;
	
	//
	// ASK FOR THE BOARD AND SOLVE IT. REPEAT.
	//
	string input;
	cout << "Enter board: " << endl;
	while ( getline( cin,input ) )
	{
		solver.solve(input);
		set<string>::const_iterator itr = solver.begin(), end = solver.end();
		for ( ; itr != end; ++itr )
		{
			cout << *itr << '\n';
		}
		
		cout << endl << endl;;
		cout << "Enter board:" << endl;
	}


	exit(0);
}

