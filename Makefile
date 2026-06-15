CC = gcc
CXX = g++

SRC_DIR = src

C_SRCS = $(wildcard $(SRC_DIR)/*.c)
CXX_SRCS = $(wildcard $(SRC_DIR)/*.cpp)

C_OBJS = $(C_SRCS:.c=.o)
CXX_OBJS = $(CXX_SRCS:.cpp=.o)
OBJS = $(C_OBJS) $(CXX_OBJS)

C_BINS = $(patsubst $(SRC_DIR)/%.c,%.elf,$(C_SRCS))
CXX_BINS = $(patsubst $(SRC_DIR)/%.cpp,%.elf,$(CXX_SRCS))
BINS = $(C_BINS) $(CXX_BINS)

CFLAGS      = -Wall -Wextra -pthread 
CXXFLAGS    = -Wall -Wextra -pthread
LDFLAGS     = -pthread

all: $(BINS)

$(C_BINS): %.elf: $(SRC_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

$(CXX_BINS): %.elf: $(SRC_DIR)/%.o
	$(CXX) $< -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
