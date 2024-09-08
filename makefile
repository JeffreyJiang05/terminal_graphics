SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

PROGRAM_NAME = prog
PROG_SRC_DIR = src/progs

FLAGS = -std=c17 -pedantic -O3 -march=native -lm -Wall -I $(INCLUDE_DIR)/
CC = gcc

SOURCE_FILES=$(shell find $(SRC_DIR)/ -maxdepth 1 -name '*.c' -printf "%f\n" | sort -k 1nr)
OBJECT_FILES=$(SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

run: $(BUILD_DIR)/$(PROGRAM_NAME)
	$(BUILD_DIR)/$(PROGRAM_NAME)

run.%: 
	$(BUILD_DIR)/$*

compile.%: $(PROG_SRC_DIR)/%.c $(OBJECT_FILES)
	$(CC) $< $(OBJECT_FILES) $(FLAGS) -o $(BUILD_DIR)/$*

$(BUILD_DIR)/$(PROGRAM_NAME): $(OBJECT_FILES)
	$(CC) $(OBJECT_FILES) $(FLAGS) -o $(BUILD_DIR)/$(PROGRAM_NAME)

$(BUILD_DIR)/%.o: $(INCLUDE_DIR) $(BUILD_DIR)
	$(CC) $(FLAGS) -c $(SRC_DIR)/$*.c -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)
clean:
	rm -rfv $(BUILD_DIR)

files:
	echo $(SOURCE_FILES)
	echo $(OBJECT_FILES)