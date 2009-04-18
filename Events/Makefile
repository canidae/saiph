CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = ChangedInventoryItems.o ReceivedItems.o

events: $(OBJECTS)

ChangedInventoryItems.o: ChangedInventoryItems.h ChangedInventoryItems.cpp
ReceivedItems.o: ReceivedItems.h ReceivedItems.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
