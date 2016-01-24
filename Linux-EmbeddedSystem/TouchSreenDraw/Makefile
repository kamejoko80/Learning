#! Makefile
CC = /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
SRC = ${wildcard *.c}
OBJ = ${SRC:%.c=%.o}

Notesbooks: TouchScreenDraw.c fbutils.c font_8x8.c 
	$(CC) $^ -o $@ -L ./lib/ -lts

.c.o:
	$(CC) -c $^

clean:
	$(RM) -f *.o Notesbooks 
