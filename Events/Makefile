CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = ReceivedItems.o

events: $(OBJECTS)

ReceivedItems.o: ReceivedItems.h ReceivedItems.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
