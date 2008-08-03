#ifndef TELNET_H
/* defines */
#define TELNET_H
/* debug */
#define TELNET_DEBUG_NAME "[Telnet     ] "
/* buffer for login */
#define TELNET_BUFFER_SIZE 4096
/* delay & attempts when reading non-blocking */
#define TELNET_NON_BLOCKING_DELAY 20000
#define TELNET_NON_BLOCKING_ATTEMPTS 100
/* packet size, used for determining if we received everything */
#define TELNET_PACKET_SIZE 1448
/* host */
#define TELNET_NETHACK_URL "nethack.alt.org"
#define TELNET_NETHACK_PORT 23

/* forward declare */
class Telnet;

/* includes */
#include <fcntl.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include "Connection.h"
#include "Globals.h"

/* namespace */
using namespace std;

/* connection to the game using telnet */
class Telnet : public Connection {
	public:
		/* constructors */
		Telnet(ofstream *debugfile);

		/* destructor */
		~Telnet();

		/* methods */
		int retrieve(char *buffer, int count, bool blocking = true);
		int transmit(const string &data);
		void start();
		void stop();

	private:
		/* variables */
		int sock;

		/* methods */
		int transmit(const char *data, int length);
};
#endif
