# run it like this:
# make clean && make && export NETHACKOPTIONS=@/home/canidae/projects/saiph/nethackrc && ./saiph

CXX = g++
RM = rm -f
CPPFLAGS =
CXXFLAGS = -O2 -Wall -g
LDFLAGS = -lutil
OBJECTS = World.o Main.o Player.o

saiph: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o saiph

World.o: World.h World.cpp
Main.o: Main.h Main.cpp
Player.o: Player.h Player.cpp

.PHONY: clean
clean:
	$(RM) saiph $(OBJECTS)
