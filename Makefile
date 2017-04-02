CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CC := g++
CFLAGS := -g -Wall -pedantic -std=gnu++98
LD_FLAGS :=

ConnectFour: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<
