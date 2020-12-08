INC= -I/usr/include
LIB= -L/usr/lib
CFLAGS=-Wall -g ${INC} -L. ${LIB}
CC=gcc

ex06.o: ex06.c
	$(CC) $(CFLAGS) -c $*.c

ex06 : ex06.o
	${CC} ${CFLAGS} -o ex06 ex06.o -lgb

clean: 
	rm -f *.o ex06 *~