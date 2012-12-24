CC=g++
FLAGS=-O3 -std=c++11 -Wall
PROJECT=wordHeroSolver
FILES=solver.cpp Makefile $(PROJECT) \
	Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c dictionary.dat \
	Compile-CRC-32-Lookup-Table.c CRC-32.dat \
	Word-List.txt

$(PROJECT): solver.o dictionary.dat
	$(CC) $(FLAGS) solver.o -o $@

solver.o: solver.cpp
	$(CC) $(FLAGS) -c $^

dictionary.dat: CRC-32.dat Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c
	gcc -g Blitzkrieg_Trie_Attack_Dawg_Creator_Custom_Character_Set.c -o builddict
	./builddict
	rm ./builddict

CRC-32.dat: Compile-CRC-32-Lookup-Table.c
	gcc -g $^ -o buildcrc
	./buildcrc
	rm ./buildcrc

tar: $(PROJECT)
	tar -cvzf WordHeroSolver_source.tar $(FILES)

clean:
	rm -f *~ *.o *.gch $(PROJECT) CRC-32.dat dictionary.dat
