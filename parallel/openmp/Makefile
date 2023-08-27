CPUC      = g++
CFLAGS    = -O3 --std=c++17 -Wall -fsanitize=address -fopenmp 
DEFS      = -D DEBUG
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = 
LINK     =  $(LIBDIR) $(LIBS)
COMPILER  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all: trees teste

trees: trees.cpp
	$(COMPILER) $(LINK) -o trees trees.cpp

teste: teste.cpp
	$(COMPILER) $(LINK) -o teste teste.cpp

clean:
	rm trees
	rm teste
