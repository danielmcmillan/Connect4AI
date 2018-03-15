H_FILES = $(wildcard src/*.h)
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
SHARED_OBJ_FILES := $(filter-out obj/AutoMarked.o obj/CommandPrompt.o obj/Tournament.o, $(OBJ_FILES))

CC = g++
CC_FLAGS = -std=gnu++11
LD_FLAGS =
wasm: CC = em++
wasm: LD_FLAGS = -s WASM=1 -s EXPORTED_FUNCTIONS='["_configure", "_computeMove", "_rowForMove", "_winningPieces"]' -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'

ifeq ($(TARGET),RELEASE)
	TARGET_CC_FLAGS = -O3 -D NDEBUG
	TARGET_LD_FLAGS =
else
	TARGET_CC_FLAGS = -g -Wall -pedantic
	TARGET_LD_FLAGS =
endif

.PHONY: cli wasm clean

cli: AutoMarked Tournament CommandPrompt

wasm: connect4ai.html

CommandPrompt: obj/CommandPrompt.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) $(TARGET_LD_FLAGS) -o $@ $^

AutoMarked: obj/AutoMarked.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) $(TARGET_LD_FLAGS) -o $@ $^

Tournament: obj/Tournament.o $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) $(TARGET_LD_FLAGS) -o $@ $^

connect4ai.html: $(SHARED_OBJ_FILES)
	$(CC) $(LD_FLAGS) $(TARGET_LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp $(H_FILES)
	$(CC) $(CC_FLAGS) $(TARGET_CC_FLAGS) -c -o $@ $<

clean:
	rm -f obj/* CommandPrompt AutoMarked Tournament connect4ai.html connect4ai.js connect4ai.wasm
