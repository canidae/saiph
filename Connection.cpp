#include "Connection.h"

/* constructors */
Connection::Connection(bool remote) {
	this->remote = remote;

	/* set up pipes */
	if (pipe(input) < 0 || pipe(output) < 0) {
		cerr << "Plumbing failed" << endl;
		exit(1);
	}

	if (remote) {
		/* TODO */
		/* play remotely */
		usleep_time = REMOTE_USLEEP;

	} else {
		/* play locally */
		usleep_time = LOCAL_USLEEP;
		/* set up local pty */
		int fd = 0;
		char slave[256];
		memset(slave, '\0', 256);
		winsize wsize;
		wsize.ws_row = ROWS;
		wsize.ws_col = COLS;
		wsize.ws_xpixel = 640;
		wsize.ws_ypixel = 480;
		pid_t pid = forkpty(&fd, slave, NULL, &wsize);
		if (pid == -1) {
			cerr << "There is no fork" << endl;
			exit(2);
		} else if (pid) {
			/* fix plumbing */
			input[1] = fd; // input == data sent to nethack
			output[0] = fd; // output == data received from nethack
			/* make reading from output pipe non-blocking */
			fcntl(output[0], F_SETFL, fcntl(output[0], F_GETFL) | O_NONBLOCK);
		} else {
			/* start nethack here */
			int result = execl(LOCAL_NETHACK, LOCAL_NETHACK, NULL);
			if (result < 0) {
				cerr << "Unable to enter the dungeon" << endl;                                                                                  
				exit(3);                                                                                                                        
			}
		}
	}
}

/* methods */
ssize_t Connection::retrieve(char *buffer, size_t count) {
	/* retrieve data */
	ssize_t data_received = 0;
	ssize_t data_received_total = 0;
	int failed = 0;
	while (data_received_total == 0 || data_received > 0 || failed < 5) {
		data_received = read(output[0], &buffer[data_received_total], count - data_received_total);
		if (data_received != -1)
			data_received_total += data_received;
		else
			++failed;
		//usleep(usleep_time);
	}
	if (data_received_total < (ssize_t) count)
		buffer[data_received_total] = '\0';
	return data_received_total;
}

ssize_t Connection::send(const char *buffer) {
	/* send data */
	return write(input[1], buffer, strlen(buffer));
}
