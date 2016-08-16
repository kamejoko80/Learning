PROG = $(file_name) 
SRC = ${PROG:%=%.cpp}
TARGET = $(PROG)

$(TARGET): $(SRC)
	g++ -o $@ $^ -lm -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv` -std=c++11

.PHONY:
clean:
	$(RM) $(TARGET)
