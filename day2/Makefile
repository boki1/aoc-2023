CXXC=g++
CXXFLAGS=-std=c++23 -ggdb3
OUT=cubeconundrum

all: cubeconundrum.cpp
	$(CXXC) $(CXXFLAGS) $< -o $(OUT)

clean:
	rm -f $(OUT)

test: all
	./$(OUT) cubeconundrum-input

.PHONY: all clean test

