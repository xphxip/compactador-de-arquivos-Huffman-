CC = gcc
CFLAGS = -Wall -Wextra -O2

OBJS = structures.o huffman.o compress.o decompress.o main.o

ifeq ($(OS),Windows_NT)
	TARGET = compressor.exe
	RM = del /Q
else
	TARGET = compressor
	RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

structures.o: structures.c structures.h
	$(CC) $(CFLAGS) -c structures.c

huffman.o: huffman.c huffman.h structures.h
	$(CC) $(CFLAGS) -c huffman.c

compress.o: compress.c compress.h huffman.h
	$(CC) $(CFLAGS) -c compress.c

decompress.o: decompress.c decompress.h huffman.h
	$(CC) $(CFLAGS) -c decompress.c

main.o: main.c compress.h decompress.h
	$(CC) $(CFLAGS) -c main.c

clean:
	$(RM) *.o $(TARGET)
