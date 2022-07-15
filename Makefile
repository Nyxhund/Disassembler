CC=gcc
CFLAGS= -O3 -std=c99 -fsanitize=address
CPPFLAGS =
LDFLAGS =
LDLIBS =

all: nyxvm

SRC = $(shell find ./*.c)
OBJ = $(SRC:.c=.o)

nyxvm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	${RM} *.o
	${RM} *.d
	${RM} nyxvm