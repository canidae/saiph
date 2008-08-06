#include "Local.h"

/* constructors */
Local::Local(ofstream *debugfile) : Connection(debugfile) {
	/* set up pipes */
	if (pipe(link) < 0) {
		*debugfile << LOCAL_DEBUG_NAME << "Plumbing failed" << endl;
		exit(1);
	}

	/* set up pty */
	int fd = 0;
	char slave[256];
	memset(slave, '\0', sizeof (slave));
	winsize wsize;
	wsize.ws_row = ROWS;
	wsize.ws_col = COLS;
	wsize.ws_xpixel = 640;
	wsize.ws_ypixel = 480;
	pid_t pid = forkpty(&fd, slave, NULL, &wsize);
	if (pid == -1) {
		*debugfile << LOCAL_DEBUG_NAME << "There is no fork" << endl;
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
			*debugfile << LOCAL_DEBUG_NAME << "Unable to enter the dungeon" << endl;
			exit(3);
		}
		return;
	}
}

/* destructor */
Local::~Local() {
	stop();
}

/* methods */
int Local::retrieve(char *buffer, int count) {
	/* retrieve data */
	ssize_t data_received = read(link[0], buffer, count);
	if (data_received >= 0)
		return (int) data_received; // if we couldn't read, data_received will be -1, which mess things up
	return 0;
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
