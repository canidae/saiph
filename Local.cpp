#include <stdlib.h>
#include <fcntl.h>

#ifdef __APPLE__
#include <util.h>
#else
#include <pty.h>
#endif

#include "Debug.h"
#include "Globals.h"
#include "Local.h"

using namespace std;

/* constructors/destructor */
Local::Local() {
	/* set up pipes */
	if (pipe(link) < 0) {
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
		link[0] = fd; // reading
		link[1] = fd; // writing
	} else {
		/* this is our pty, start nethack here */
		int result;
		setenv("TERM", "xterm", 1);
		result = execl(LOCAL_NETHACK, LOCAL_NETHACK, "TERM=xterm", NULL);
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
int Local::retrieve(char *buffer, int count) {
	/* retrieve data */
	ssize_t data_received = 0;
	/* make reading blocking */
	fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) & ~O_NONBLOCK);
	/* read 8 bytes, this will block until there's data available */
	data_received += read(link[0], buffer, 8);
	/* usleep some ms here (after the blocked reading) both to
	 * make sure that we've received all the data and to make the
	 * game watchable  */
	usleep(200000);
	/* make reading non-blocking */
	fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) | O_NONBLOCK);
	data_received += read(link[0], &buffer[data_received], count - data_received - 2);
	if (data_received < (ssize_t) count)
		buffer[data_received] = '\0';
	return (int) data_received;
}

int Local::transmit(const string &data) {
	/* send data */
	return (int) write(link[1], data.c_str(), data.size());
}

void Local::start() {
	/* no need for some special code here */
}

void Local::stop() {
	transmit("Sy"); // save & quit
}
