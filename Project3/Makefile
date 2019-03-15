CFLAGS = -std=c99 -g -Wall -Wshadow --pedantic -Wunreachable-code -Wvla -Werror -Wunreachable-code -lm
COVFLAGS = 
PROFFLAG = 
GCC = gcc $(CFLAGS) $(COVFLAGS) $(PROFFLAG)

all: pathfinder.o
	$(GCC) pathfinder.o -o pathfinder
.c.o: 
	$(GCC) -c $*.c

testall:all
	./pathfinder inputs/map5x5.txt inputs/query5x5.txt
	./pathfinder inputs/usa.txt inputs/usa1.txt
	./pathfinder inputs/usa.txt inputs/usa10.txt
	./pathfinder inputs/usa.txt inputs/usa100.txt
memory:all
	valgrind --tool=memcheck --leak-check=full ./pathfinder inputs/map5x5.txt inputs/query5x5.txt
	valgrind --tool=memcheck --leak-check=full ./pathfinder inputs/usa.txt inputs/usa1.txt
	valgrind --tool=memcheck --leak-check=full ./pathfinder inputs/usa.txt inputs/usa10.txt
	valgrind --tool=memcheck --leak-check=full ./pathfinder inputs/usa.txt inputs/usa100.txt
clean:
	/bin/rm -f *.o
	/bin/rm -f pathfinder
