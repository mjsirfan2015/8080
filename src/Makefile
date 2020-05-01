CC=g++#compiler
TARGET=dbg
all:
	$(CC) cpu.h dbg.cpp -o   $(TARGET) -lSDL2 -pthread
	./$(TARGET)
clean:
	rm $(TARGET)