CPUC      = g++
CFLAGS    = -O3 --std=c++17 -Wall -fsanitize=address
DEFS      =
INCLUDES  =
LIBDIR   = -L/usr/lib
LIBS     = 
LINK     =  $(LIBDIR) $(LIBS)
COMPILER  = $(CPUC) $(DEFS) $(INCLUDES) $(CFLAGS)

all:
	$(COMPILER) $(LINK) -o trees trees.cpp
	$(COMPILER) $(LINK) -o teste teste.cpp

clean:
	rm trees
	rm teste
