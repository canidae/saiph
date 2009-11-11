#ifndef TELNET_H
#define TELNET_H
/* debug */
#define TELNET_DEBUG_NAME "Telnet] "
/* buffer for login */
#define TELNET_BUFFER_SIZE 4096
/* host */
#define TELNET_NETHACK_PORT 23

#include <string>
#include "Connection.h"

class Telnet : public Connection {
public:
	Telnet();
	virtual ~Telnet();

	virtual int retrieve(char* buffer, int count);
	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

private:
	int _sock;
	char _ping[3];

	int transmit(const char* data, int length);
};
#endif
