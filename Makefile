# =========================================================================
# Makefile to complete for the first project (info0027)
#
# You can add your own rules but YOU CANNOT EDIT compilation flags.
# 
# =========================================================================
CC=gcc
CFLAGS=--std=c99 --pedantic -Wall -W -Wmissing-prototypes -DNDEBUG
LDFLAGS=

SRC=radix.c wordPuzzle.c solver.c loader.c
OBJ=$(SRC:.c=.o)

EXEC=solver

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

clean:
	@rm -rf *.o
	@rm -rf $(EXEC)

mrproper: clean
	@rm -rf $(EXEC)

run: $(EXEC)
	./$(EXEC) test_dic.txt test_grid.txt

