#ifndef CONNECTION_H
#define CONNECTION_H
/* connection interfaces */
#define CONNECTION_LOCAL 1
#define CONNECTION_TELNET 2

#include <fstream>
#include <string>

class Connection {
public:
	Connection();
	virtual ~Connection();

	static Connection *create(int interface);

	virtual int retrieve(char *buffer, int count);
	virtual int transmit(const std::string &data);
	virtual void start();
	virtual void stop();
};
#endif
