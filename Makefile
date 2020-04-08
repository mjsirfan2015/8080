CC=g++#compiler
TARGET=main
all:
	$(CC) src/cpu.h src/disas.h main.cpp -o  $(TARGET)
	./main
clean:
	rm $(TARGET)