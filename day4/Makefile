CXXC := g++
CXXFLAGS := -std=c++23 -ggdb3
SRC := $(wildcard *.cpp)
OUT := $(patsubst %.cpp,%,$(SRC))

all: $(SRC)
	$(CXXC) $(CXXFLAGS) $< -o $(OUT)

clean:
	rm -f $(OUT)

test: all
	gdb -tui -ex start --args ./$(OUT) simple-input

submit: all
	./$(OUT) input

.PHONY: all clean test test submit

