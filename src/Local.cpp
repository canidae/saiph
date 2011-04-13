#include "Local.h"

#include <stdlib.h>
#include <fcntl.h>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

#include "Debug.h"
#include "Globals.h"

using namespace std;

/* constructors/destructor */
Local::Local() : _unanswered_chars(1), _synchronous(0) {
	/* set up pipes */
	if (pipe(_link) < 0) {
		Debug::error() << LOCAL_DEBUG_NAME << "Plumbing failed" << endl;
		exit(1);
	}

	/* set up pty */
	int fd = 0;
	char slave[256] = {0};
	winsize wsize;
	wsize.ws_row = ROWS;
	wsize.ws_col = COLS;
	wsize.ws_xpixel = 640;
	wsize.ws_ypixel = 480;
	pid_t pid = forkpty(&fd, slave, NULL, &wsize);
	if (pid == -1) {
		Debug::error() << LOCAL_DEBUG_NAME << "There is no fork" << endl;
		exit(1);
	} else if (pid) {
		/* main thread */
		/* fix plumbing */
		_link[0] = fd; // reading
		_link[1] = fd; // writing
	} else {
		/* this is our pty, start nethack here */
		int result;
		setenv("TERM", "xterm", 1);
		setenv("SAIPH_INLINE_SYNC", "1", 1);
		result = execl("/bin/sh", "sh", "-c", LOCAL_NETHACK, NULL);
		if (result < 0) {
			Debug::error() << LOCAL_DEBUG_NAME << "Unable to enter the dungeon" << endl;
			exit(3);
		}
		return;
	}
}

Local::~Local() {
	stop();
}

/* methods */
int Local::removeThorns(char *buffer, int count) {
	int writep = 0;
	int readp = 0;
	int removed = 0;

	while (readp != count) {
		char ch = buffer[readp++];
		if (ch == (char)0xFE)
			removed++;
		else
			buffer[writep++] = ch;
	}

	return removed;
}

int Local::retrieve(char* buffer, int count) {
	/* retrieve data */
	ssize_t data_received = 0;
	ssize_t amount;
	if (_synchronous > 0) {
		do {
			amount = read(_link[0], &buffer[data_received], count - data_received - 2);
			if (amount > 0) {
				int th = removeThorns(&buffer[data_received], amount);
				data_received -= th;
				_unanswered_chars -= th;
				data_received += amount;
			}
		} while (amount >= 0 && _unanswered_chars);
	} else {
		/* make reading blocking */
		//fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) & ~O_NONBLOCK);
		/* read 8 bytes, this will block until there's data available */
		//data_received += read(link[0], buffer, 8);
		/* make reading non-blocking */
		fcntl(_link[0], F_SETFL, fcntl(_link[0], F_GETFL) | O_NONBLOCK);
		/* usleep some ms here (after the blocked reading) both to
		 * make sure that we've received all the data and to make the
		 * game watchable  */
		usleep(20000);
		do {
			amount = read(_link[0], &buffer[data_received], count - data_received - 2);
			if (amount > 0)
				data_received += amount;
		} while (amount > 1000 && !(amount & (amount + 1))); // power of 2 test
		if (data_received > 0) {
			int th = removeThorns(buffer, data_received);
			if (th > 0) {
				data_received -= th;
				_unanswered_chars = 0;
				_synchronous = 1;
				Debug::info() << LOCAL_DEBUG_NAME << "Using inband syncronization" << endl;
				fcntl(_link[0], F_SETFL, fcntl(_link[0], F_GETFL) & ~O_NONBLOCK);
			}
		}
	}
	if (data_received < (ssize_t) count)
		buffer[data_received] = '\0';
	return (int) data_received;
}

int Local::transmit(const string& data) {
	/* send data */
	_unanswered_chars += data.size();
	return (int) write(_link[1], data.c_str(), data.size());
}

void Local::start() {
	/* no need for some special code here */
}

void Local::stop() {
	transmit("Sy"); // save & quit
}
