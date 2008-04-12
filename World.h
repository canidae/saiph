#ifndef WORLD_H
/* defines */
#define WORLD_H
/* text telling us there are more messages */
#define MORE "--More--"
#define MORE_LENGTH 8
/* we need these to determine if we're looking at a menu */
#define END "(end)"
#define END_LENGTH 6 // seems to be a bug here, there's an space after; "(end) "
#define PAGE " of "
#define PAGE_LENGTH 6 // must add 2 because of "y)" in "(x of y)"

/* forward declare */
class World;

/* includes */
#include <string>
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
		bool question;
		bool menu;

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
		string msg_str; // used for fetching messages

		/* methods */
		void handleEscapeSequence(int &pos, int &colour);
		void fetchMessages();
		void fetchMessagesHelper(int row, int startcol);
		void update();
};
#endif
