SOURCE_DIR := src
BUILD_DIR := bin

CC := g++ -c
LD := g++

# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

CFLAGS := $(shell sdl2-config --cflags)
LDFLAGS := $(shell sdl2-config --libs) -lGL -lstdc++

SRCS := $(call rwildcard,$(SOURCE_DIR)/,*.cpp)
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:$(SOURCE_DIR)/%.cpp=%.o))

MAIN = test

all: mkbin link

mkbin:
	mkdir -p $(BUILD_DIR)

link: $(OBJS)
	$(LD) -o $(MAIN) $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(wildcard $(SOURCE_DIR)/%.h)
	$(CC) -o $@ $< $(CFLAGS)

clean:
	rm $(OBJS) $(MAIN)



