#ifndef LOCAL_H
/* defines */
#define LOCAL_H
/* debug */
#define LOCAL_DEBUG_NAME "[      Local] "
/* buffer */
#define READ_LIMIT 4095
#define BUFFER_SIZE 65536
/* local */
#define LOCAL_NETHACK "/usr/games/nethack"
#define LOCAL_USLEEP 0

/* forward declare */
class Local;

/* includes */
#include <fcntl.h>
#include <iostream>
#include <string>
#include <pty.h>
#include "Connection.h"
#include "Globals.h"

/* namespace */
using namespace std;

/* connection with local game */
class Local : public Connection {
	public:
		/* constructors */
		Local(ofstream *debugfile);

		/* destructor */
		~Local();

		/* methods */
		virtual int retrieve(char *buffer, int count);
		virtual int transmit(const string &data);
		virtual void start();
		virtual void stop();

	private:
		/* variables */
		int link[2];
		unsigned long usleep_time;
};
#endif
