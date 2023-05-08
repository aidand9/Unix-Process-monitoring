CC = gcc
CFLAGS = -Werror -Wall -std=c99

assignment2: specific_process.o process_handling.o main.o
	$(CC) $(CFLAGS) -o assignment2 specific_process.o process_handling.o main.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c


specific_process.o: specific_process.c specific_process.h
	$(CC) $(CFLAGS) -c specific_process.c

process_handling.o: process_handling.c process_handling.h
	$(CC) $(CFLAGS) -c process_handling.c


clean:
	rm *.o