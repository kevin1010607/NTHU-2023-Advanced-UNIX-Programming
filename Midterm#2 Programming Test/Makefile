CC = gcc
CFLAGS = -std=c11 -O2
TARGETS = q1 q2 q3 q4

.PHONY: all
all: $(TARGETS)

%: %.c
	$(CC) -o $@ $(CFLAGS) $<

.PHONY: clean
clean:
	rm -f $(TARGETS)
