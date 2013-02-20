CC=g++
CXXFLAGS=-O3 -std=c++11 -Wall
C=gcc
CFLAGS=
PROJECT=wordHeroSolver
FILES=$(PROJECT)  Makefile  \
	src/solver.cpp src/Char.h src/Dawg.h src/Word-List.txt \
	includes/Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c dictionary.dat \
	includes/Compile-CRC-32-Lookup-Table.c \
	

$(PROJECT): solver.o Char.o Dwag.o dictionary.dat
	$(CC) $(CXXFLAGS) $< -o $@

solver.o: src/solver.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $^

Char.o: src/Char.h
	$(CC) $(CXXFLAGS) -c -o $@ $^

Dwag.o: src/Dawg.h
	$(CC) $(CXXFLAGS) -c -o $@ $^

dictionary.dat: includes/Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c CRC-32.dat 
	$(C) $(CFLAGS) $< -o builddict
	cp src/Word-List.txt .
	./builddict > /dev/null
	rm ./Word-List.txt
	rm ./builddict

CRC-32.dat: includes/Compile-CRC-32-Lookup-Table.c
	$(C) $(CFLAGS) $^ -o buildcrc
	./buildcrc >/dev/null
	rm ./buildcrc

tar: $(PROJECT)
	tar -cvzf WordHeroSolver_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT) CRC-32.dat dictionary.dat
