CC = gcc
CFLAG = -std=c11 -O2 -Wall
TARGET = mock
SRCS = mock.c
OBJS = mock.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAG) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
