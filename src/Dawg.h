
#ifndef _DAWG_H
#define _DAWG_H

#include <fstream>

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

int* initDawg( const char* file )
{
	int numberOfNodes, *dawgArray;
	std::ifstream input( file, std::fstream::binary | std::fstream::in );
	if ( !input )
	{
		return NULL;
	}
	input.read( (char*)&numberOfNodes, sizeof(int) );
	dawgArray = new int[numberOfNodes];
	input.read( (char*)dawgArray, numberOfNodes*sizeof(int) );
	input.close();

	return dawgArray;
}


#endif