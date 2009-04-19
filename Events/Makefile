CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = Event.o

events: $(OBJECTS)

Event.o: Event.h Event.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
