CC = g++
CFLAGS = -Wall -g -O2 -std=c++17
SRC_DIR = . SkipList Json BufferPool
SRCFLAGS = $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.cpp) $(wildcard $(dir)/*.cc) $(wildcard $(dir)/*.c))

OBJECTS = $(patsubst $.cpp, $.o, $(SRCFLAGS))

INCFLGS = -I./SkipList
INCFLGS = -I./Json
INCFLGS = -I./BufferPool

LIBS = 

LIBFLGS = 

all: new.out

new: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(INCFLGS) $(LIBFLGS) $(LIBS) -lpthread

clean:
	rm *.out