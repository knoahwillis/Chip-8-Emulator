PNAME := Chip-8

PCXXSRC := $(wildcard src/*.cpp)

PCXXOBJ := $(PCXXSRC:.cpp=.o)

CXXFLAGS += -std=c++17 -O2 -g -Wall -Wpedantic

LDLIBS += 

LDFLAGS += -lSDL2 

CC := g++

.PHONY: all clean 

all: $(PCXXOBJ)
	$(CC) $(LDLIBS) $(PCXXOBJ) $(LDFLAGS) -o $(PNAME)
	$(RM) $(PCXXOBJ)

%.o: %.cpp
	$(CC) -c $< $(CXXFLAGS) -o $@

clean:
	$(RM) $(PNAME) 
	$(RM) src/*.o
