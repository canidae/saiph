# run it like this:
# make clean && make && export NETHACKOPTIONS=@/home/canidae/projects/saiph/nethackrc && ./saiph

CXX = g++
RM = rm -f
CPPFLAGS =
CXXFLAGS = -O2 -Wall -g
LDFLAGS = -lutil
OBJECTS = Connection.o Player.o Saiph.o World.o

saiph: $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o saiph

Connection.o: Connection.h Connection.cpp
Player.o: Player.h Player.cpp
Saiph.o: Saiph.h Saiph.cpp
World.o: World.h World.cpp

.PHONY: clean
clean:
	$(RM) saiph $(OBJECTS)
