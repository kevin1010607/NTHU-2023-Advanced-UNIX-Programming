CC = gcc
CFLAGS = -std=c11 -O2 -Wall
TARGETS = assignment6

.PHONY: all
all: $(TARGETS)

%: %.c
	$(CC) -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f $(TARGETS)
