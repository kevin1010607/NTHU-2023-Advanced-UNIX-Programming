CC = gcc
CFLAGS = -std=c11 -O2 -Wall
TARGETS = mock mock_v2

.PHONY: all
all: $(TARGETS)

%: %.c
	$(CC) -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f $(TARGETS)
