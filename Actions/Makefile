CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = Move.o

events: $(OBJECTS)

Move.o: Move.h Move.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
