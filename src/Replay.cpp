#include "Replay.h"
#include "Debug.h"
#include "World.h"

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

using namespace std;

std::string Replay::_filename;
std::ostream *Replay::_recordstream;
bool Replay::_recording;
int sequence;

/* constructors/destructor */
Replay::Replay() {
	_replaystream.open(_filename.c_str(), ifstream::in | ifstream::binary);
}

Replay::~Replay() {
}

/* static methods */
void Replay::setFile(const string& filename, bool recording) {
	_filename = filename;
	_recording = recording;

	if (_recording)
		_recordstream = new ofstream(_filename.c_str(), ofstream::out | ofstream::binary);
}

void Replay::recordFrame(char *buffer, int count) {
	if (!_recording)
		return;
	char header[12];
	// faked a bit
	long long timestamp = 300000LL * (sequence++);
	long microsec = (long) (timestamp % 1000000LL);
	long sec = (long) (timestamp / 1000000LL);

	header[ 0] = char((microsec >> 0) & 0xFF);
	header[ 1] = char((microsec >> 8) & 0xFF);
	header[ 2] = char((microsec >> 16) & 0xFF);
	header[ 3] = char((microsec >> 24) & 0xFF);
	header[ 4] = char((sec >> 0) & 0xFF);
	header[ 5] = char((sec >> 8) & 0xFF);
	header[ 6] = char((sec >> 16) & 0xFF);
	header[ 7] = char((sec >> 24) & 0xFF);
	header[ 8] = char((count >> 0) & 0xFF);
	header[ 9] = char((count >> 8) & 0xFF);
	header[10] = char((count >> 16) & 0xFF);
	header[11] = char((count >> 24) & 0xFF);

	_recordstream->write(header, 12);
	_recordstream->write(buffer, count);
	_recordstream->flush();
}

/* methods */
int Replay::doRetrieve(char* buffer, int bsize) {
	char header[12];
	_replaystream.read(header, 12);
	if (_replaystream.gcount() != 12)
		return 0;
	int amount =
		(static_cast<unsigned char> (header[11]) << 24) +
		(static_cast<unsigned char> (header[10]) << 16) +
		(static_cast<unsigned char> (header[9]) << 8) +
		(static_cast<unsigned char> (header[8]));
	if (amount > bsize) {
		Debug::error() << "Mismatched buffer size in Replay::doRetrieve" << endl;
		World::destroy();
		exit(1);
	}
	_replaystream.read(buffer, amount);
	if (amount < bsize)
		buffer[amount] = 0;
	return amount;
}

int Replay::transmit(const string& data) {
	return data.size();
}

void Replay::start() {
}

void Replay::stop() {
}
