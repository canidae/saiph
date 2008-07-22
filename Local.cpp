#include "Local.h"

/* constructors */
Local::Local(ofstream *debugfile) : Connection(debugfile) {
	/* set up pipes */
	if (pipe(link) < 0) {
		*debugfile << LOCAL_DEBUG_NAME << "Plumbing failed" << endl;
		exit(1);
	}

	usleep_time = LOCAL_USLEEP;
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
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = LOCAL_USLEEP;
	fd_set rfds;
	FD_ZERO(&rfds);
	FD_SET(link[0], &rfds);

	/* see if there is any data for us */
	int retval = select(link[0] + 1, &rfds, NULL, NULL, NULL);
	if (retval <= 0)
		return 0;
	/* read data if any */
	int data_read = (int) read(link[0], buffer, count);
	/* we need to set the last char to '\0',
	 * or we'll get issue when we string it later */
	buffer[data_read] = '\0';
	return data_read;
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
