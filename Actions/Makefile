CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = Action.o

events: $(OBJECTS)

Action.o: Action.h Action.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
