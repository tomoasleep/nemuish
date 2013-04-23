CFLAGS = -Wall -O
CC = gcc
LDLIBS = -lreadline -lncurses
TARGET = nemuish
SRCS := $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
RM = rm -f

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	$(RM) $(TARGET) $(OBJS) *~

