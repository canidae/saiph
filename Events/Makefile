CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = EventBus.o

events: $(OBJECTS)

EventBus.o: EventBus.h EventBus.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
