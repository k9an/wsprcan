CC=gcc

CFLAGS= -I/opt/local/include -Wall -O3 -ffast-math -Wno-missing-braces
LDFLAGS = -L/opt/local/lib 
LIBS = -lfftw3 -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:    k9an-wsprd wsprsim

DEPS =  wsprsim_utils.h wsprd_utils.h fano.h jelinek.h

OBJS2 = wsprsim.o wsprsim_utils.o wsprd_utils.o tab.o fano.o nhash.o
wsprsim: $(OBJS2)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

OBJS3 = wsprd.o wsprsim_utils.o wsprd_utils.o tab.o fano.o jelinek.o nhash.o
k9an-wsprd: $(OBJS3)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

clean:
	rm *.o k9an-wsprd wsprsim
