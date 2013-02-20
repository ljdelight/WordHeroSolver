
#ifndef _WORDHEROSOLVER_H
#define _WORDHEROSOLVER_H

#include "Char.h"
#include "Dawg.h"
#include <string>
#include <set>
#include <iostream>
#include <cassert>

struct sortByStringLength {
    bool operator() (const std::string& lhs, const std::string& rhs) const{
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

class WordHeroSolver 
{
public:
	WordHeroSolver( int NRows = 4, int NCols = 4 )
		: nRows(NRows), nCols(NCols) 
	{
		// TODO(ljdelight): verify nrows/ncols are positive. Else, throw exception
		board = new Char[nRows*nCols];
		dawg = initDawg( "dictionary.dat" );
		if ( !dawg )
		{
			std::cerr << "could not initialize the DAWG (bad file?)" << std::endl;
			exit(1);
		}
	}
	// TODO(ljdelight): dtor 

	void solve( std::string input )
	{
		const int MAX_STRING_LENGTH = 50;
		wordsFound.clear();
		for ( size_t i = 0; i < input.size(); ++i )
		{
			board[i] = Char( input[i] );
		}

		char str[MAX_STRING_LENGTH];
		for ( int i = 0; i < nRows; ++i )
		{
			for ( int j = 0; j < nCols; ++j )
			{
				solvehelper( str, 0, MAX_STRING_LENGTH, i, j );
			}
		}
	}
	std::set<std::string>::const_iterator begin() { return wordsFound.begin(); }
	std::set<std::string>::const_iterator end() { return wordsFound.end(); }
private:
	int nRows;
	int nCols;
	Char* board;
	int* dawg;
	std::set<std::string, sortByStringLength> wordsFound;


	const int MAX_STRING_LENGTH = 50;
	bool isInBounds( int x, int y )
	{
		return (x>=0 && x<nRows && y>=0 && y<nCols);
	}
	bool isValidCell( int x, int y )
	{
		return isInBounds(x,y) && ! board[x*nCols+y].isVisited();
	}


	void solvehelper( char* str, int strLen, const int maxStrLen,
					int x, int y, int dawgIdx = 0 )
	{
		if ( ! isValidCell( x, y ) )
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

		solvehelper( str, strLen, maxStrLen, x-1, y-1, dawgIdx );  // upper-left
		solvehelper( str, strLen, maxStrLen, x-1, y,   dawgIdx );  // upper-center
		solvehelper( str, strLen, maxStrLen, x-1, y+1, dawgIdx );  // upper-right
		solvehelper( str, strLen, maxStrLen, x,   y-1, dawgIdx );  // left
		solvehelper( str, strLen, maxStrLen, x,   y+1, dawgIdx );  // right
		solvehelper( str, strLen, maxStrLen, x+1, y-1, dawgIdx );  // bottom-left
		solvehelper( str, strLen, maxStrLen, x+1, y,   dawgIdx );  // bottom-center
		solvehelper( str, strLen, maxStrLen, x+1, y+1, dawgIdx );  // bottom-right

		// remove the char from the string and set to unvisited
		strLen -= 1;
		board[idx].setNotVisited();
	}
};

#endif
