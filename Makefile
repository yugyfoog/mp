CXXFLAGS = -Wall -Wextra -g -Ofast
CFLAGS = -Wall -Wextra -g -Ofast
ASFLAGS = -g

MPLIB = mp.o fast.o multiply.o schoolboy.o
MPHEAD = mp.h fast.h multiply.h schoolboy.h

all: mptest

mptest: mptest.o $(MPLIB)
	g++ -g -o mptest mptest.o $(MPLIB)

mptest.o: $(MPHEAD)

$(MPLIB): $(MPHEAD)

