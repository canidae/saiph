#ifndef WORLD_H
/* defines */
#define WORLD_H
/* where we can find various stuff */
#define MAP_ROW_START 1 // where the actual map start
#define MAP_ROW_END 21 // and end
#define ATTRIBUTES_ROW 22
#define STATUS_ROW 23
/* text telling us there are more messages */
#define MORE "--More--"
/* we don't have enough characters to make unique symbols for every tile,
 * these values are supposed to help on that */
#define OPEN_DOOR -128
#define TREE -127
#define IRON_BARS -126
#define THRONE -125
#define SINK -124
#define LAVA -123
#define ICE -122
#define LOWERED_DRAWBRIDGE -121
#define PET -120
#define PLAYER -119

/* forward declare */
class World;

/* includes */
#include "Connection.h"
#include "Dungeon.h"
#include "Player.h"

/* namespace */
using namespace std;

/* the world in our eyes */
class World: public Dungeon {
	public:
		/* variables */
		char data[BUFFER_SIZE];
		ssize_t data_size;

		/* constructors */
		World(Connection *connection);

		/* destructors */
		~World();

		/* methods */
		void command(const char *command);

	private:
		/* variables */
		int row; // cursor position, row
		int col; // cursor position, col
		Connection *connection;
		int messages_pos; // used for concatenation multiple messages together

		/* methods */
		void handleEscapeSequence(int &pos, int &colour);
		void fetchMessages();
		void fetchMessagesHelper(int row, int startcol);
		void update();
};
#endif
