CC       := gcc
OBJDUMP  := objdump
CFLAGS   := -Wall -Wextra -O2
LIBS     := -lSDL3 -lm

SRC_DIR  := src
BUILD_DIR:= build
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/main
DISASM   := $(BUILD_DIR)/main.asm

SRCS     := $(wildcard $(SRC_DIR)/*.c)
OBJS     := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(TARGET) $(DISASM)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(DISASM): $(TARGET)
	$(OBJDUMP) -d $< > $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean