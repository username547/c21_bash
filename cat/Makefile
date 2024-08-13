CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11

EXECUTABLE = s21_cat

all: $(EXECUTABLE)

$(EXECUTABLE): main.o cat.o
	$(CC) $(FLAGS) main.o cat.o -o $(EXECUTABLE)

main.o: main.c cat.h
	$(CC) $(FLAGS) -c main.c

cat.o: cat.c cat.h
	$(CC) $(FLAGS) -c cat.c

clean:
	rm -f $(EXECUTABLE) *.o

rebuild: clean all

test:
	./cat_tests.sh
