# Set clang as C compiler
CC = clang
CXX = clang++

# Compiler flags
CFLAGS = -Wall
CXXFLAGS = $(CFLAGS) -std=c++11

# Target file
TARGET = nta_receiver

OBJS = $(TARGET) socket.o tcp_socket.o udp_socket.o 

all: $(OBJS)

$(TARGET): $(TARGET).cpp socket.o tcp_socket.o udp_socket.o 
	$(CXX) $(CXXFLAGS) -lpthread -o $(OBJS) $(TARGET).cpp

tcp_socket.o: tcp_socket.cpp socket.hpp

udp_socket.o: udp_socket.cpp socket.hpp

arm: CFLAGS += -target arm-linux-gnueabihf -mcpu=cortex-a7 --sysroot=/usr/arm-linux-gnueabihf -I/usr/arm-linux-gnueabihf/include -I/usr/arm-linux-gnueabihf/include/c++/5/arm-linux-gnueabihf -L/usr/arm-linux-gnueabihf/lib
arm: all

clean:
	$(RM) $(OBJS)