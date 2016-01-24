CC = /usr/local/arm-2009q3/bin/arm-none-linux-gnueabi-gcc
SRC = ${wildcard *.c}
OBJ = ${SRC:%.c=%.o}

fbtest: $(OBJ)
	$(CC) $^ -o $@

.c.o:
	$(CC) -c $^

clean:
	$(RM) -f *.o fbtest


