PROGRAM=serwificonf
CC=mips-openwrt-linux-gcc #gcc
CFLAGS=
SOURCES=main.c serial.c

all: $(PROGRAM)

$(PROGRAM): $(SOURCES)
	$(CC) $(SOURCES) -o $(PROGRAM)

clean:
	rm -rf $(PROGRAM)

.PHONY: all clean

