#ifndef WORLD_H
/* defines */
#define WORLD_H
/* text telling us there are more messages */
#define MORE "--More--"

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
		char *data;
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
