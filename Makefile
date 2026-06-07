CC = gcc
CXX = g++

SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:.c=.o)

BINS = $(patsubst $(SRC_DIR)/%.c,%.elf,$(SRCS))

CFLAGS      = -Wall -Wextra -pthread 
CXXFLAGS    = -Wall -Wextra -pthread
LDFLAGS     = -pthread

all: $(BINS)

%.elf: $(SRC_DIR)/%.o
	$(CC) $< -o $@ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
