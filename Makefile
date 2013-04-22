CFLAGS = -Wall -O
CC = gcc
TARGET = nemuish
SRCS := $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
RM = rm -f

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET) $(OBJS) *~
