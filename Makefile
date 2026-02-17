CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra
LDFLAGS =

BUILD_DIR = build
TARGET = $(BUILD_DIR)/book-tracker

SRCS = main.cpp \
       src/core/data-types/data-types-operations.cpp \
       src/core/disk/disk-operations.cpp \
       src/cli/cli-operations.cpp

HEADERS = $(shell find . -name "*.hpp")

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR)

run: all
	./$(TARGET)

.PHONY: all clean run