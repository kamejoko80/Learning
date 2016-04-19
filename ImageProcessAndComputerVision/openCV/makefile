PROG = $(file_name) 
SRC = ${PROG:%=%.cpp}
TARGET = $(PROG)

$(TARGET): $(SRC)
	g++ -o $@ $^ -lm -ggdb `pkg-config --cflags opencv` `pkg-config --libs opencv`

.PHONY:
clean:
	$(RM) $(TARGET)
