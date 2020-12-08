netINC= -I/usr/include
LIB= -L/usr/lib
CFLAGS=-Wall -g ${INC} -L. ${LIB}
CC=gcc

ex07.o: ex07.c
	$(CC) $(CFLAGS) -c $*.c

ex07 : ex07.o
	${CC} ${CFLAGS} -o ex07 ex07.o -lgb

clean: 
	rm -f *.o ex07 *~