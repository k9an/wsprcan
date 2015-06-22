CC = gcc
#CC = clang
FC = gfortran

FFLAGS = -O2 -Wall -Wno-conversion
CFLAGS= -g -I/usr/include -Wall -Wno-missing-braces -O2
LDFLAGS = -L/usr/lib
LIBS = -lfftw3 -lm

# Default rules
%.o: %.c $(DEPS)
	${CC} ${CFLAGS} -c $<
%.o: %.f
	${FC} ${FFLAGS} -c $<
%.o: %.F
	${FC} ${FFLAGS} -c $<
%.o: %.f90
	${FC} ${FFLAGS} -c $<
%.o: %.F90
	${FC} ${FFLAGS} -c $<

all:    k9an-wsprd wsprsim 

DEPS =  wsprsim_utils.h wsprd_utils.h fano.h

OBJS2 = wsprsim.o wsprsim_utils.o wsprd_utils.o tab.o fano.o nhash.o 
wsprsim: $(OBJS2) 
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)

OBJS3 = wsprd.o wsprsim_utils.o wsprd_utils.o tab.o fano.o nhash.o 
k9an-wsprd: $(OBJS3)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
clean:
	rm *.o k9an-wsprd wsprsim
