#include "Local.h"

/* constructors */
Local::Local() {
	/* set up pipes */
	if (pipe(link) < 0) {
		cerr << "Plumbing failed" << endl;
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
		cerr << "There is no fork" << endl;
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
			cerr << "Unable to enter the dungeon" << endl;
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
	ssize_t data_received = 0;
	ssize_t data_received_total = 0;
	/* make reading blocking */
	fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) & ~O_NONBLOCK);
	/* read 4 bytes, this will block until there's data available */
	data_received_total += 4;
	data_received = read(link[0], buffer, data_received_total);
	/* make reading non-blocking */
	fcntl(link[0], F_SETFL, fcntl(link[0], F_GETFL) | O_NONBLOCK);
	data_received = read(link[0], &buffer[data_received_total], count - data_received_total);
	if (data_received != -1)
		data_received_total += data_received;
	while (data_received_total % READ_LIMIT == 0 && data_received != -1) {
		/* it seems to return max 4095 chars */
		usleep(100000);
		data_received = read(link[0], &buffer[data_received_total], count - data_received_total);
		if (data_received != -1)
			data_received_total += data_received;
	}
	if (data_received_total < (ssize_t) count)
		buffer[data_received_total] = '\0';
	return (int) data_received_total;
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
