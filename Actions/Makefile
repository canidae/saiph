CXX = g++
RM = rm -f
CXXFLAGS = -O0 -Wall -Wextra -g3 -gdwarf-2
OBJECTS = Action.o Kick.o Move.o PutOn.o Remove.o

events: $(OBJECTS)

Action.o: Action.h Action.cpp
Move.o: Kick.h Kick.cpp
Move.o: Move.h Move.cpp
PutOn.o: PutOn.h PutOn.cpp
Remove.o: Remove.h Remove.cpp

.PHONY: clean
clean:
	$(RM) *.o *.gch
