CC=gcc
CFLAGS= -O3 -std=c99 -fsanitize=address
CPPFLAGS =
LDFLAGS =
LDLIBS =

all: disassembler

SRC = $(shell find ./*.c)
OBJ = $(SRC:.c=.o)

disassembler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LDLIBS)

clean:
	${RM} *.o
	${RM} *.d
	${RM} disassembler
