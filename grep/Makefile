CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

EXECUTABLE = s21_grep

all: $(EXECUTABLE)

$(EXECUTABLE): main.o grep.o
	$(CC) $(FLAGS) main.o grep.o -o $(EXECUTABLE)

main.o: main.c grep.h
	$(CC) $(FLAGS) -c main.c

grep.o: grep.c grep.h
	$(CC) $(FLAGS) -c grep.c

clean: 
	rm -f *.o $(EXECUTABLE)

rebuild: clean all

test:
	./grep_tests.sh
