#ifndef LOCAL_H
#define LOCAL_H
/* debug */
#define LOCAL_DEBUG_NAME "Local] "
/* buffer */
#define READ_LIMIT 4095
#define BUFFER_SIZE 65536
/* local */
#define LOCAL_NETHACK "nethack"

#include <string>
#include "Connection.h"

class Local : public Connection {
public:
	Local();
	virtual ~Local();

	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

protected:
	virtual int doRetrieve(char* buffer, int count);

private:
	int _link[2];
	int _unanswered_chars;
	int _synchronous;
	int removeThorns(char* buffer, int count);
};
#endif
