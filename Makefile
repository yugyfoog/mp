CFLAGS = -Wall -g -O3
CXXFLAGS = -Wall -g -O3

HEADERS = mp.h mpint.h mpfloat.h digits.h fast.h

mptest: mptest.o mpint.o mp.o fast.o
	g++ -o mptest mptest.o mpint.o mp.o fast.o

mptest.o: $(HEADERS)

clean:
	rm -f *.o *~ *.stackdump
