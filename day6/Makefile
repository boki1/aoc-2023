CXXC := g++
CXXFLAGS := -std=c++23 -ggdb3
SRC := $(wildcard *.cpp)
OUT := $(patsubst %.cpp,%,$(SRC))
INPUT ?= simple-input

all: $(SRC)
	$(CXXC) $(CXXFLAGS) $< -o $(OUT)

clean:
	rm -f $(OUT)

test: all
	gdb -tui -ex start --args ./$(OUT) $(INPUT)

submit: all
	$(CXXC) -O3 -std=c++23 $< -o $(OUT)
	./$(OUT) input

.PHONY: all clean test test submit

