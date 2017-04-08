H_FILES = $(wildcard src/*.h)
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
SHARED_OBJ_FILES := $(filter-out obj/AutoMarked.o obj/CommandPrompt.o obj/Tournament.o, $(OBJ_FILES))

CC = g++
CFLAGS = -g -Wall -pedantic -std=gnu++98
LD_FLAGS =

.PHONY: all clean

all: AutoMarked CommandPrompt

AutoMarked: obj/AutoMarked.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

Tournament: obj/Tournament.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

CommandPrompt: obj/CommandPrompt.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp $(H_FILES)
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -f obj/* Automarked Tournament CommandPrompt
