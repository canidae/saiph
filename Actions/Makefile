CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g
OBJECTS = Action.o

events: $(OBJECTS)

Action.o: Action.h Action.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
