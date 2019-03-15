CFLAGS = -g -Wall -Wshadow --pedantic -Wunreachable-code -Wvla -Werror
COVFLAGS = 
PROFFLAG = 
GCC = gcc $(CFLAGS)$(COVFLAGS) $(PROFFLAG)

ch: huff.o compress.o
	$(GCC) huff.o compress.o -o myhuff
cu: unhuff.o compress.o
	$(GCC) unhuff.o compress.o -o myunhuff
.c.o: 
	$(GCC) -c $*.c

testh:ch
	./myhuff inputs/text0.txt
	./myhuff inputs/text1.txt
	./myhuff inputs/text2.txt
	./myhuff inputs/text3.txt
	./myhuff inputs/text4.txt
	./myhuff inputs/text5.txt

testu:cu
	./myunhuff inputs/text0.txt.huff
	./myunhuff inputs/text1.txt.huff
	./myunhuff inputs/text2.txt.huff
	./myunhuff inputs/text3.txt.huff
	./myunhuff inputs/text4.txt.huff
	./myunhuff inputs/text5.txt.huff

memory:testh
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text0.txt
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text1.txt
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text2.txt
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text3.txt
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text4.txt
	valgrind --tool=memcheck --leak-check=full ./myhuff inputs/text5.txt
clean:
	/bin/rm -f *.o
	/bin/rm -f myhuff
	/bin/rm -f text0.txt.huff
	/bin/rm -f text1.txt.huff
	/bin/rm -f text2.txt.huff
	/bin/rm -f text3.txt.huff
	/bin/rm -f text4.txt.huff
	/bin/rm -f text5.txt.huff
	/bin/rm -f text0.txt.huff.unhuff
	/bin/rm -f text1.txt.huff.unhuff
	/bin/rm -f text2.txt.huff.unhuff
	/bin/rm -f text3.txt.huff.unhuff
	/bin/rm -f text4.txt.huff.unhuff
	/bin/rm -f text5.txt.huff.unhuff
