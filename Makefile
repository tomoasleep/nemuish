CFLAGS = -Wall -O
CC = gcc
LDLIBS = -lreadline -lncurses
TARGET = nemuish
SRCS := $(wildcard *.c)
# SRCS := main.c parse.c print.c 
OBJS = $(SRCS:.c=.o)
RM = rm -f

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

debug: clean 
	make CFLAGS="-Wall -g"

clean:
	$(RM) $(TARGET) $(OBJS) *~

.c.o:
	$(CC) $(CFLAGS) -c $<

parse.c: print.c
main.c: parse.c

