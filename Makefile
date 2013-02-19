CC=g++
CXXFLAGS=-O3 -std=c++11 -Wall
C=gcc
CFLAGS=
PROJECT=wordHeroSolver
FILES=solver.cpp Makefile $(PROJECT) \
	Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c dictionary.dat \
	Compile-CRC-32-Lookup-Table.c CRC-32.dat \
	Word-List.txt

$(PROJECT): solver.o dictionary.dat
	$(CC) $(CXXFLAGS) $< -o $@

solver.o: src/solver.cpp
	$(CC) $(CXXFLAGS) -c -o $@ $^


dictionary.dat: src/Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c CRC-32.dat 
	$(C) $(CFLAGS) $< -o builddict
	cp src/Word-List.txt .
	./builddict > /dev/null
	rm ./Word-List.txt
	rm ./builddict

CRC-32.dat: src/Compile-CRC-32-Lookup-Table.c
	$(C) $(CFLAGS) $^ -o buildcrc
	./buildcrc >/dev/null
	rm ./buildcrc

tar: $(PROJECT)
	tar -cvzf WordHeroSolver_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT) CRC-32.dat dictionary.dat
