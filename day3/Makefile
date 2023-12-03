CXXC=g++
CXXFLAGS=-std=c++23 -ggdb3
OUT=gearratios

all: gearratios.cpp
	$(CXXC) $(CXXFLAGS) $< -o $(OUT)

clean:
	rm -f $(OUT)

test: all
	gdb -tui -ex start --args ./$(OUT) simple-input

.PHONY: all clean test

