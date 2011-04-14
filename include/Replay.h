#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <string>
#include "Connection.h"

class Replay : public Connection {
public:
	Replay();
	virtual ~Replay();

	virtual int transmit(const std::string& data);
	virtual void start();
	virtual void stop();

	static void setFile(const std::string& file, bool recording);
	static void recordFrame(char *buffer, int count);

protected:
	virtual int doRetrieve(char* buffer, int count);

private:
	static std::string _filename;
	static bool _recording;
	static std::ostream *_recordstream;
	static int _sequence;

	std::ifstream _replaystream;
};
#endif
