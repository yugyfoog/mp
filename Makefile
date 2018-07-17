CFLAGS = -Wall -g -O3
CXXFLAGS = -Wall -g -O3

HEADERS = mp.h mpint.h mpfloat.h digits.h

mptest: mptest.o mpint.o
	g++ -o mptest mptest.o mpint.o

mptest.o: $(HEADERS)

clean:
	rm -f *.o *~ *.stackdump
