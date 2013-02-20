
#ifndef _WORDHEROSOLVER_H
#define _WORDHEROSOLVER_H

#include "Char.h"
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
	WordHeroSolver( int NRows = 4, int NCols = 4 );
	// TODO(ljdelight): dtor 

	void solve( std::string input );
	std::set<std::string>::const_iterator begin() const;
	std::set<std::string>::const_iterator end() const;
private:
	int nRows;
	int nCols;
	Char* board;
	int* dawg;
	std::set<std::string, sortByStringLength> wordsFound;
	const int MAX_STRING_LENGTH = 50;

	bool isInBounds( int x, int y );
	bool isValidCell( int x, int y );
	void solvehelper( char* str, int strLen, const int maxStrLen,
					int x, int y, int dawgIdx = 0 );
};

#endif
