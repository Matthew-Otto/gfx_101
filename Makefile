CC       := gcc
CFLAGS   := -Wall -Wextra -g
LIBS     := -lSDL3 -lm

SRC_DIR  := src
BUILD_DIR:= build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/main

SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean