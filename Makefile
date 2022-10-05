# Compiler
CC = g++

# Linker
LD = g++

# Build debug version
debug: MFLAGS += debug
debug: CFLAGS += -Wall -Wextra -DDEBUG -std=c++17 -g -O0
debug: LDFLAGS += -lstdc++fs
debug: task2.1

# Build release version
release: MFLAGS += release
release: CFLAGS += -DNDEBUG
release: task2.1

# List sources and target object files 
SOURCES = $(wildcard *.cpp)
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

# Build
task2.1: $(OBJECTS)
	$(LD) -o task2.1 $(OBJECTS) $(LDFLAGS)

# Compile
%.o: %.c 
	$(CC) -c $(CFLAGS) -o $@ $<

# Clean phony target to remove binary files
.PHONY: clean
clean:
	rm -rf *.o task2.1

