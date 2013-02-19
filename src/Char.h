

#ifndef _CHAR_H
#define _CHAR_H

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

#endif
