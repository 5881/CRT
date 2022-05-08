LDFLAGS=-lmpsse
CFLAGS= -O3 -fopenmp `pkg-config opencv4 --cflags --libs`
CFLAGS2= -O3 
CC=g++
all: clean black text

black:
	$(CC) $(CFLAGS) black.c -o black $(LDFLAGS)

text:
	$(CC) $(CFLAGS2) text.c -o text $(LDFLAGS)

sin:
	$(CC) $(CFLAGS2) sin.c -o sin $(LDFLAGS)

clean:
	rm -f black text sin

distclean: clean
