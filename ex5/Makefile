INC= -I/usr/include
LIB= -L/usr/lib
CFLAGS=-Wall -g ${INC} -L. ${LIB}
CC=gcc

ex05.o: ex05.c
	$(CC) $(CFLAGS) -c $*.c

ex05 : ex05.o
	${CC} ${CFLAGS} -o ex05 ex05.o -lgb

clean: 
	rm -f *.o ex05 *~