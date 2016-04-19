CFLAGS = -Wall -O3 `pkg-config --cflags opencv`

LIBS = `pkg-config --libs opencv`

main:test.o 
	g++ -std=c++11 -g -o main test.o  $(LIBS) -lpthread
test.o:test.cpp initialization.h process.h
	g++ -std=c++11 -mcpu=cortex-a15 -mfpu=neon -mapcs  -marm -c $(CFLAGS) test.cpp 

.phony:clean

clean:
	rm -rf ./*.o ./a.out
