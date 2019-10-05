APP	= ./bin/mod_info.bin
CC	= $(CROSS_COMPILE)gcc

CFLAGS += -Wall
CFLAGS += ./src/mod_info/mod_info.c

all:
	$(CC) $(CFLAGS) -o $(APP)

.PHONY: clean
clean:
	rm $(APP)
