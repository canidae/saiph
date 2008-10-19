#ifndef TELNET_H
#define TELNET_H
/* debug */
#define TELNET_DEBUG_NAME "[Telnet     ] "
/* buffer for login */
#define TELNET_BUFFER_SIZE 4096
/* host */
#define TELNET_NETHACK_PORT 23

#include <fstream>
#include <string>
#include "Connection.h"

class Telnet : public Connection {
	public:
		Telnet(std::ofstream *debugfile);
		~Telnet();

		int retrieve(char *buffer, int count);
		int transmit(const char *data, int length);
		int transmit(const std::string &data);
		void start();
		void stop();

	private:
		int sock;
		char ping[3];
};
#endif
