#ifndef EMBED_H
#define EMBED_H

#include <string>
#include <vector>
#include <deque>

#include <pthread.h>

#include "Connection.h"

class Embed : public Connection {
public:
	Embed();
	virtual ~Embed();

	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

protected:
	virtual int doRetrieve(char* buffer, int count);

private:
	static pthread_t threadid;
	static pthread_mutex_t mutex;
	static pthread_cond_t ready;

	static std::deque<char> inputq;
	static std::vector<char> outputq;
	static int iwaiting, exited, used;

	static int hook_input();
	static void hook_output(const char*, int);
	static void hook_exit(int);
	static void* startNetHack(void*);
};
#endif
