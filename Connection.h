#ifndef CONNECTION_H
/* defines */
#define CONNECTION_H
/* buffer */
#define READ_LIMIT 4095
#define BUFFER_SIZE 65536
/* local */
#define LOCAL_NETHACK "/usr/games/nethack"
#define LOCAL_USLEEP 0
/* remote */
#define REMOTE_NETHACK_URL "nethack.alt.org"
#define REMOTE_NETHACK "/usr/bin/telnet"
#define REMOTE_USLEEP 200000

/* forward declare */
class Connection;

/* includes */
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <pty.h>
#include "Globals.h"

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
		ssize_t send(const string &buffer);

	private:
		/* variables */
		int link[2];
		unsigned long usleep_time;
};
#endif
