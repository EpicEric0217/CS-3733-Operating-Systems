CC = gcc
CFLAGS = -Wall -g

all: driver1 driver2

clean:
	rm -f *.o driver1 driver2

driver1.o: driver1.c myio.h
	$(CC) $(CFLAGS) -c driver1.c

driver2.o: driver2.c myio.h
	$(CC) $(CFLAGS) -c driver2.c

myio.o: myio.c myio.h
	$(CC) $(CFLAGS) -c myio.c

driver1: driver1.o myio.o
	$(CC) $(CFLAGS) -o driver1 driver1.o myio.o

driver2: driver2.o myio.o
	$(CC) $(CFLAGS) -o driver2 driver2.o myio.o
