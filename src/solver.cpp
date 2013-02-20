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

#include "Char.h"
#include "Dawg.h"
using namespace std;

const int N_ROWS = 4;
const int N_COLS = 4;
const int MAX_STRING_LENGTH = 50;


bool isInBounds( int x, int y, int nRows, int nCols )
{
	return (x>=0 && x<nRows && y>=0 && y<nCols);
}
bool isValidCell( int x, int y, int nRows, int nCols, Char* board )
{
	return isInBounds(x,y,nRows,nCols) && ! board[x*nCols+y].isVisited();
}

struct sortByStringLength {
    bool operator() (const string& lhs, const string& rhs) const{
        const size_t lhs_len = lhs.length(),
        		     rhs_len = rhs.length();
    	if ( lhs_len == rhs_len )
        {
        	return lhs < rhs;
        }
    	else
    	{
    		return lhs_len < rhs_len;
    	}
    }
};


void solvehelper( set<string,sortByStringLength>& wordsFound, 
				Char* board, const int nRows, const int nCols,
				char* str, int strLen, const int maxStrLen,
				int x, int y,
				int* dawg, int dawgIdx = 0 )
{
	if ( ! isValidCell( x, y, nRows, nCols, board ) )
	{
		return;
	}

	int idx = x*nCols + y;
	char theCurrentChar = board[idx].getChar();

	// strLen==0 when we're starting a new word.
	if ( strLen == 0 )
	{
		// The DAWG indices for 'A'-'Z' are on [1,26] which is one-based.
		dawgIdx = theCurrentChar - 'A' + 1;
	}
	else
	{
		// The DAWG_CHILD is a list of chars that can follow the previous char.
		//    Search the list to see if the char was found (making a valid prefix)
		dawgIdx = DAWG_CHILD(dawg, dawgIdx);
		while ( dawgIdx && DAWG_LETTER(dawg, dawgIdx) != theCurrentChar )
		{
			dawgIdx = DAWG_NEXT(dawg, dawgIdx);
		}
		// if the char was not found then we have an INVALID prefix! stop recursion
		if ( !dawgIdx )
		{
			return;
		}
	}

	// we must have the board's character equal to the dawgIdx char value.
	assert( theCurrentChar == DAWG_LETTER(dawg,dawgIdx) );

	// place the character in the string
	str[strLen] = theCurrentChar;
	board[idx].setVisited();
	strLen += 1;

	// strLen is the number of readable chars (excluding terminating)
	//   So strLen must always be on the range [0,maxStrLen-1)
	assert( strLen < maxStrLen-1 );

	if ( strLen>=3 && DAWG_END_OF_WORD(dawg, dawgIdx) )
	{
		str[strLen] = '\0';
		wordsFound.insert(str);
	}

	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x-1, y-1, dawg, dawgIdx );  // upper-left
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x-1, y,   dawg, dawgIdx );  // upper-center
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x-1, y+1, dawg, dawgIdx );  // upper-right
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x,   y-1, dawg, dawgIdx );  // left
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x,   y+1, dawg, dawgIdx );  // right
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x+1, y-1, dawg, dawgIdx );  // bottom-left
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x+1, y,   dawg, dawgIdx );  // bottom-center
	solvehelper( wordsFound, board, nRows, nCols, str, strLen, maxStrLen, x+1, y+1, dawg, dawgIdx );  // bottom-right

	// remove the char from the string and set to unvisited
	strLen -= 1;
	board[idx].setNotVisited();
}


void solve( set<string, sortByStringLength>& wordsFound, int* dawg, Char* board, string input )
{
	for ( size_t i = 0; i < input.size(); ++i )
	{
		board[i] = Char( input[i] );
	}

	char str[MAX_STRING_LENGTH];
	for ( int i = 0; i < N_ROWS; ++i )
	{
		for ( int j = 0; j < N_COLS; ++j )
		{
			solvehelper( wordsFound, board, N_ROWS, N_COLS, str, 0, MAX_STRING_LENGTH, i, j, dawg );
		}
	}
}

int main( int argc, char* argv[] )
{
	int *dawg = initDawg( "dictionary.dat" );
	if ( !dawg )
	{
		cerr << "could not initialize the DAWG (bad file?)" << endl;
		exit(1);
	}

	Char* board = new Char[N_ROWS*N_COLS];
	set<string, sortByStringLength> wordsFound;

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

	
	//
	// ASK FOR THE BOARD AND SOLVE IT. REPEAT.
	//
	string input;
	cout << "Enter board: " << endl;
	while ( getline( cin,input ) )
	{
		solve( wordsFound, dawg, board, input);

		auto itr = wordsFound.begin(), end = wordsFound.end();
		for ( ; itr != end; ++itr )
		{
			cout << *itr << '\n';
		}
		wordsFound.clear();
		cout << endl << endl;;
		cout << "Enter board:" << endl;
	}


	delete [] dawg;
	delete [] board;

	exit(0);
}

