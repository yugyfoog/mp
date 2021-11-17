CXXFLAGS = -Wall -Wextra -g -Ofast
CFLAGS = -Wall -Wextra -g -Ofast
ASFLAGS = -g

MPLIB = mp.o fast.o multiply.o schoolboy.o
MPHEAD = mp.h fast.h multiply.h schoolboy.h

all: mptest chudnovsky

mptest: mptest.o $(MPLIB)
	g++ -g -o mptest mptest.o $(MPLIB)

chudnovsky: chudnovsky.o $(MPLIB)
	g++ -g -o chudnovsky chudnovsky.o $(MPLIB)

mptest.o chudnovsky.o $(MPLIB): $(MPHEAD)

clean:
	rm -f *.o *.~
