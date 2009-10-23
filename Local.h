#ifndef LOCAL_H
#define LOCAL_H
/* debug */
#define LOCAL_DEBUG_NAME "Local] "
/* buffer */
#define READ_LIMIT 4095
#define BUFFER_SIZE 65536
/* local */
#define LOCAL_NETHACK "/usr/games/nethack"

#include <string>
#include "Connection.h"

class Local : public Connection {
public:
	Local();
	virtual ~Local();

	virtual int retrieve(char* buffer, const int& count);
	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

private:
	int _link[2];
};
#endif
