#ifndef LOCAL_H
/* defines */
#define LOCAL_H
/* debug */
#define LOCAL_DEBUG_NAME "[Local      ] "
/* local */
#define LOCAL_NETHACK "/usr/games/nethack"
/* delay & attempts when reading non-blocking */
#define LOCAL_NON_BLOCKING_DELAY 2000
#define LOCAL_NON_BLOCKING_ATTEMPTS 100

/* forward declare */
class Local;

/* includes */
#include <fcntl.h>
#include <iostream>
#include <pty.h>
#include <string>
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
};
#endif
