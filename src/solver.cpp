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

using namespace std;

const int N_ROWS = 4;
const int N_COLS = 4;
const int MAX_STRING_LENGTH = 50;


// These defs are from http://www.pathcom.com/~vadco/dawg.html
//  A HUGE thank you goes out to JohnPaul Adamovsky for creating this beast
//
#define CHILD_BIT_SHIFT 10
#define END_OF_WORD_BIT_MASK 0X00000200
#define END_OF_LIST_BIT_MASK 0X00000100
#define LETTER_BIT_MASK 0X000000FF
#define DAWG_LETTER(thearray, theindex) (thearray[theindex]&LETTER_BIT_MASK)
#define DAWG_END_OF_WORD(thearray, theindex) (thearray[theindex]&END_OF_WORD_BIT_MASK)
#define DAWG_NEXT(thearray, theindex) ((thearray[theindex]&END_OF_LIST_BIT_MASK)? 0: theindex + 1)
#define DAWG_CHILD(thearray, theindex) (thearray[theindex]>>CHILD_BIT_SHIFT)

int* initDawg( const char* file)
{
	int numberOfNodes, *dawgArray;
	std::ifstream input( file, std::fstream::binary | std::fstream::in );
	if ( !input )
	{
		return NULL;
	}
	input.read( (char*)&numberOfNodes, sizeof(int));
	dawgArray = new int[numberOfNodes];
	input.read( (char*)dawgArray, numberOfNodes*sizeof(int));
	input.close();

	return dawgArray;
}



class Char
{
private:
	bool visited;
	char character;
public:
	Char() : visited(true),character(-1) {}
	Char( char theCharValue ) : visited(false), character(theCharValue) {}

	bool isVisited() const { return visited; }
	char getChar() const { return character; }

	void setVisited() { visited = true; }
	void setNotVisited() { visited = false; }
};
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
set<string, sortByStringLength> wordsFound;


void solver( Char* board, const int nRows, const int nCols,
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

	solver( board, nRows, nCols, str, strLen, maxStrLen, x-1, y-1, dawg, dawgIdx );  // upper-left
	solver( board, nRows, nCols, str, strLen, maxStrLen, x-1, y,   dawg, dawgIdx );  // upper-center
	solver( board, nRows, nCols, str, strLen, maxStrLen, x-1, y+1, dawg, dawgIdx );  // upper-right
	solver( board, nRows, nCols, str, strLen, maxStrLen, x,   y-1, dawg, dawgIdx );  // left
	solver( board, nRows, nCols, str, strLen, maxStrLen, x,   y+1, dawg, dawgIdx );  // right
	solver( board, nRows, nCols, str, strLen, maxStrLen, x+1, y-1, dawg, dawgIdx );  // bottom-left
	solver( board, nRows, nCols, str, strLen, maxStrLen, x+1, y,   dawg, dawgIdx );  // bottom-center
	solver( board, nRows, nCols, str, strLen, maxStrLen, x+1, y+1, dawg, dawgIdx );  // bottom-right

	// remove the char from the string and set to unvisited
	strLen -= 1;
	board[idx].setNotVisited();
}




int main( int argc, char* argv[] )
{
	if ( argc != 2 )
	{
		cerr << "Missing args: solver <dictionary_BIN>" << endl;
		exit(1);
	}

	int *dawg = initDawg(argv[1]);
	if ( !dawg )
	{
		cerr << "could not initialize the DAWG (bad file?)" << endl;
		exit(1);
	}

	Char* board = new Char[N_ROWS*N_COLS];
	char str[MAX_STRING_LENGTH];

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
	while ( getline( cin,input) )
	{
		for ( size_t i = 0; i < input.size(); ++i )
		{
			board[i] = Char( input[i] );
		}

		for ( int i = 0; i < N_ROWS; ++i )
		{
			for ( int j = 0; j < N_COLS; ++j )
			{
				solver( board, N_ROWS, N_COLS, str, 0, MAX_STRING_LENGTH, i, j, dawg );
			}
		}
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

