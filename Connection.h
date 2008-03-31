#ifndef CONNECTION_H
/* defines */
#define CONNECTION_H
/* local */
#define LOCAL_NETHACK "/usr/games/nethack"
#define LOCAL_USLEEP 5000 // 5ms
/* remove */
#define REMOTE_NETHACK ""
#define REMOTE_USLEEP 25000 // 25ms

/* forward declare */
class Connection;

/* includes */
#include <fcntl.h>
#include <iostream>
#include <pty.h>
#include "Dungeon.h"

/* namespace */
using namespace std;

/* connection handles communication with a local/remote game */
class Connection {
	public:
		/* variables */
		bool remote;

		/* constructors */
		Connection(bool remote);

		/* methods */
		ssize_t retrieve(char *buffer, size_t count);
		ssize_t send(const char *buffer);

	private:
		/* variables */
		int input[2];
		int output[2];
		unsigned long usleep_time;
};
#endif
