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

	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

protected:
	virtual int doRetrieve(char* buffer, int count);

private:
	int _sock;
	char _ping[3];

	int transmit(const char* data, int length);
};
#endif
