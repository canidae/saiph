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
	while (transmit(data, 47) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard data in socket

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
	/* calculate roughly our latency
	 * this is the intriguing bit:
	 * when we get a new send request, usleep(expected_latency).
	 * then check if there's any data in the socket.
	 * if it is, then don't send the command.
	 * why not? because it means that we didn't get all the data last time,
	 * so we just want to read the rest */
	*debugfile << TELNET_DEBUG_NAME << "Last send: " << last_send.tv_sec << "." << last_send.tv_usec << " | Last receive: " << last_receive.tv_sec << "." << last_receive.tv_usec << endl;
	expected_latency = (expected_latency + ((last_receive.tv_sec - last_send.tv_sec) * 1000000 + last_receive.tv_usec - last_send.tv_usec)) / 2;
	expected_latency = expected_latency * 4 / 3 + 5000; // add 33% and 5ms

	if (retrieved != 0 && retrieved % TELNET_PACKET_SIZE == 0) {
		/* if we get packets of size 1448 we probably didn't get it all */
		*debugfile << TELNET_DEBUG_NAME << "Received " << retrieved << " bytes, expecting more data" << endl;
		/* we'll have to set "last_send" here.
		 * it's complex to explain, but:
		 * when we receive a packet, the server awaits our "ack".
		 * this "ack" is sent immediately when we received the last packet.
		 * so the latency will be ~correct when we set "last_send" here.
		 * we really just want the time it took from we sent a packet and until we received one */
		gettimeofday(&last_send, NULL);
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
		retrieve(discard, TELNET_BUFFER_SIZE); // discard data in socket
	while (transmit(username) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard data in socket
	while (transmit(password) == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard data in socket
	while (transmit("p") == 0)
		retrieve(discard, TELNET_BUFFER_SIZE); // discard data in socket
}

void Telnet::stop() {
}

/* private methods */
int Telnet::transmit(const char *data, int length) {
	*debugfile << TELNET_DEBUG_NAME << "Sleeping " << expected_latency << " microseconds" << endl;
	usleep(expected_latency);
	*debugfile << TELNET_DEBUG_NAME << "Sending: '" << data << "'" << endl;
	/* let's peek */
	char buffer[2];
	int retrieved = recv(sock, buffer, 2, MSG_DONTWAIT | MSG_PEEK);
	if (retrieved > 0) {
		*debugfile << TELNET_DEBUG_NAME << "Data in the socket, not sending command" << endl;
		return 0; // there's data, don't send
	}
	gettimeofday(&last_send, NULL);
	return send(sock, data, length, 0);
}
