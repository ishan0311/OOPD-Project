CXX = g++
ASM = nasm

INCLUDES = -Iinclude

SRC = src/main.cpp \
      src/basicIO.cpp \
      src/Utils.cpp \
      src/Protocol.cpp \
      src/UserDevice.cpp \
      src/CellTower.cpp \
      src/CellularCore.cpp

CXXFLAGS_RELEASE = -std=c++17 -O2 -Wall -Wextra -pthread
CXXFLAGS_DEBUG   = -std=c++17 -g  -O0 -Wall -Wextra -pthread

all: simulator_opt simulator_debug

# Optimized build
simulator_opt: syscall.o
	$(CXX) -nostartfiles $(CXXFLAGS_RELEASE) $(INCLUDES) $(SRC) syscall.o -o simulator_opt

# Debug build
simulator_debug: syscall.o
	$(CXX) -nostartfiles $(CXXFLAGS_DEBUG) $(INCLUDES) $(SRC) syscall.o -o simulator_debug

# Assemble syscall backend
syscall.o: syscall.S
	$(ASM) -f elf64 syscall.S -o syscall.o

clean:
	rm -f src/*.o *.o simulator simulator_opt simulator_debug syscall.o
