CXXC=g++
CXXFLAGS=-std=c++23 -ggdb3
OUT=trebuchet

all: trebuchet.cpp
	$(CXXC) $(CXXFLAGS) $< -o $(OUT)

clean:
	rm -f $(OUT)

test-simple1: all
	./$(OUT) test-input

test-simple2: all
	./$(OUT) test-input

test: all
	./$(OUT) trebuchet-input

.PHONY: all clean test

