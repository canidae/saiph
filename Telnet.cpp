#include "Telnet.h"
#include <sys/time.h>
#include <time.h>

/* constructors */
Telnet::Telnet(ofstream *debugfile) : Connection(debugfile) {
	/* reset some variables */
	gettimeofday(&last_send, NULL);
	gettimeofday(&last_receive, NULL);
	expected_latency = 500000;
	/* connect to host using telnet */
	struct hostent *he = gethostbyname(TELNET_NETHACK_URL);
	if (he == NULL) {
		herror("gethostbyname");
		exit(1);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket");
		exit(1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TELNET_NETHACK_PORT);
	addr.sin_addr = *((struct in_addr *) he->h_addr);
	memset(addr.sin_zero, '\0', sizeof (addr.sin_zero));

	if (connect(sock, (struct sockaddr *) &addr, sizeof (addr)) == -1) {
		perror("connect");
		exit(1);
	}

	/* tell server our terminal (xterm-color) and size (80x24) and set some other things */
	char data[] = {0xff, 0xfb, 0x18, 0xff, 0xfa, 0x18, 0x00, 'x', 't', 'e', 'r', 'm', 0xff, 0xf0, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x23, 0xff, 0xfc, 0x27, 0xff, 0xfe, 0x03, 0xff, 0xfb, 0x01, 0xff, 0xfd, 0x05, 0xff, 0xfb, 0x21, 0xff, 0xfb, 0x1f, 0xff, 0xfa, 0x1f, 0x00, 0x50, 0x00, 0x18, 0xff, 0xf0}; 
	/* buffer used for discarding */
	char discard[TELNET_BUFFER_SIZE];
	retrieve(discard, TELNET_BUFFER_SIZE); // discard initial response
	while (transmit(data, 47) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard unexpected data
	retrieve(discard, TELNET_BUFFER_SIZE); // discard reply to setting modes
	retrieve(discard, TELNET_BUFFER_SIZE); // discard menu

	/* and let's log in */
	start();
}

/* destructor */
Telnet::~Telnet() {
	stop();
	close(sock);
}

/* methods */
int Telnet::retrieve(char *buffer, int count) {
	/* retrieve data */
	*debugfile << TELNET_DEBUG_NAME << "Read requested" << endl;
	int retrieved = recv(sock, buffer, count, 0);
	/* set last_receive */
	gettimeofday(&last_receive, NULL);
	/* calculate roughly our latency */
	expected_latency = (expected_latency + timediff(last_send, last_receive)) / 2;
	/* and set last_send even if we didn't send anything.
	 * why? because we might send an ACK to the server.
	 * we'll also overwrite this value the next time we "transmit".
	 * it's probably "ok" */
	gettimeofday(&last_send, NULL);

	if (retrieved != 0 && retrieved % TELNET_PACKET_SIZE == 0) {
		/* if we get packets of size 1448 we probably didn't get it all */
		*debugfile << TELNET_DEBUG_NAME << "Received " << retrieved << " bytes, expecting more data" << endl;
		retrieved += retrieve(&buffer[retrieved], count - retrieved);
	}
	*debugfile << TELNET_DEBUG_NAME << "Read " << retrieved << " bytes: ";
	for (int a = 0; a < retrieved; ++a)
		*debugfile << buffer[a];
	*debugfile << endl;

	if (retrieved >= 0)
		return retrieved;
	return 0;
}

int Telnet::transmit(const string &data) {
	return transmit(data.c_str(), data.length());
}

void Telnet::start() {
	ifstream account;
	account.open(".account");
	if (!account) {
		*debugfile << TELNET_DEBUG_NAME << "Unable to read .account (which should contain username and password on separate lines)" << endl;
		exit(1);
	}
	string username;
	string password;
	account >> username;
	account >> password;
	account.close();
	username.append("\n");
	password.append("\n");
	char discard[TELNET_BUFFER_SIZE];
	while (transmit("l") == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard unexpected data
	sleep(1);
	while (transmit(username) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard unexpected data
	sleep(1);
	while (transmit(password) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard unexpected data
	sleep(1);
	while (transmit("p") == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard unexpected data
}

void Telnet::stop() {
}

/* private methods */
long Telnet::timediff(const timeval &first, const timeval &last) {
	return (long) (last.tv_sec - first.tv_sec) * 1000000 + last.tv_usec - first.tv_usec;
}

int Telnet::transmit(const char *data, int length) {
	*debugfile << TELNET_DEBUG_NAME << "Sending: '" << data << "'" << endl;
	*debugfile << TELNET_DEBUG_NAME << "Expected latency: " << expected_latency << endl;
	long max_latency = expected_latency * 6 / 5 + 5000;
	stop_time = last_receive;
	stop_time.tv_sec += max_latency / 1000000;
	stop_time.tv_usec += max_latency % 1000000;
	*debugfile << TELNET_DEBUG_NAME << "Stop time: " << stop_time.tv_sec << "." << stop_time.tv_usec << endl;
	gettimeofday(&current_time, NULL);
	*debugfile << TELNET_DEBUG_NAME << "Current time: " << current_time.tv_sec << "." << current_time.tv_usec << endl;
	*debugfile << TELNET_DEBUG_NAME << "Diff: " << timediff(current_time, stop_time) << endl;
	while (gettimeofday(&current_time, NULL) == 0 && timediff(current_time, stop_time) > 0) {
		/* let's peek */
		char buffer[2];
		int retrieved = recv(sock, buffer, 2, MSG_DONTWAIT | MSG_PEEK);
		if (retrieved > 0) {
			*debugfile << TELNET_DEBUG_NAME << "Data in the socket, not sending command" << endl;
			return 0; // there's data, don't send
		}
		/* no data? sleep and try again */
		*debugfile << "Sleeping " << max_latency / 100 << " microseconds" << endl;
		usleep(max_latency / 100);
	}
	/* no data in socket, we probably got it all */
	gettimeofday(&last_send, NULL);
	return send(sock, data, length, 0);
}
