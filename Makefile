H_FILES = $(wildcard src/*.h)
OBJ_FILES := $(addprefix obj/,$(notdir $(H_FILES:.h=.o)))

CC = g++
CFLAGS = -g -Wall -pedantic -std=gnu++98
LD_FLAGS =

.PHONY: all clean

all: AutoMarked CommandPrompt

AutoMarked: obj/AutoMarked.o $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

# Tournament: obj/Tournament.o $(OBJ_FILES)
# 	$(CC) $(LD_FLAGS) -o $@ $^

CommandPrompt: obj/CommandPrompt.o $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp $(H_FILES)
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -f obj/* Automarked Tournament CommandPrompt
