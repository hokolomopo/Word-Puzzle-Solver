# =========================================================================
# Makefile to complete for the first project (info0027)
#
# You can add your own rules but YOU CANNOT EDIT compilation flags.
# 
# =========================================================================
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=

SRC=radix.c solver.c
OBJ=$(SRCS:.c=.o)

EXEC=solver

all:$(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)

run:
	./$(EXEC)

