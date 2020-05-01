CC=g++#compiler
TARGET=main
all:
	$(CC) src/cpu.h src/disas.h display/draw.h main.cpp -o  $(TARGET) -lSDL2 -pthread
	./$(TARGET)
clean:
	rm $(TARGET)