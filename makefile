CC = gcc

CFLAGS = -std=c17 -Werror -Wextra -I/usr/include/SDL2 -D_REENTRANT

LDFLAGS = -L/usr/lib -lSDL2 -lm

TARGET = rt

SRCS = main.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean