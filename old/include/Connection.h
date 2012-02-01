#ifndef CONNECTION_H
#define CONNECTION_H
/* connection interfaces */
#define CONNECTION_LOCAL 1
#define CONNECTION_TELNET 2
#define CONNECTION_REPLAY 3
#define CONNECTION_EMBED 4

#include <fstream>
#include <string>

class Connection {
public:
	Connection();
	virtual ~Connection();

	static Connection* create(int interface);

	int retrieve(char* buffer, int count);
	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

protected:
	virtual int doRetrieve(char* buffer, int count);
};
#endif
