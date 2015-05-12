CC=gcc
CFLAGS= -I/usr/include -Wall -O2 -Wno-missing-braces
LDFLAGS = -L/usr/lib 
LIBS = -lfftw3 -lm

DEPS = fano.h wsprd_utils.h
OBJ = wsprd.o wsprd_utils.o fano.o tab.o nhash.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

k9an-wsprd: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean: 
	rm *.o
