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
	usleep(200000);
	ssize_t data_received = 0;
	/* make reading blocking */
	fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) & ~O_NONBLOCK);
	/* read 4 bytes, this will block until there's data available */
	data_received += read(link[0], buffer, 4);
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
