#ifndef TELNET_H
#define TELNET_H
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
	bool _vt_tiledata2011;
	int _overs_pending;

	int transmit(const char* data, int length);
	int scanOvers(const char* data, int length);
	int removeOvers(char* data, int length);
};
#endif
