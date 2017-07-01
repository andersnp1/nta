# Set clang as C compiler
CC = clang

# Compiler flags
CFLAGS = -Wall

# Target file
TARGET = broadcaster

OBJS = $(TARGET) options.o socket.o 

all: $(OBJS) 

$(TARGET): $(TARGET).c options.o socket.o
	$(CC) $(CFLAGS) -o $(OBJS) $(TARGET).c

options.o: options.c
	$(CC) $(CFLAGS) -c options.c

socket.o: socket.c options.h
	$(CC) $(CFLAGS) -c socket.c

clean:
	$(RM) $(OBJS)