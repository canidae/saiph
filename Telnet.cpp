#include "Telnet.h"

/* constructors */
Telnet::Telnet(ofstream *debugfile) : Connection(debugfile) {
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
	transmit(data, 47);

	/* that last command we sent to will send us data back.
	 * let's discard it */
	char discard[4096];
	retrieve(discard, 4096);

	/* and let's log in */
	start();
}

/* destructor */
Telnet::~Telnet() {
	stop();
	close(sock);
}

/* methods */
int Telnet::retrieve(char *buffer, int count, bool blocking) {
	/* retrieve data */
	int retrieved = 0;
	if (blocking) {
		/* make reading blocking */
		fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) & ~O_NONBLOCK);
		/* read 4 bytes blocked */
		retrieved = recv(sock, buffer, 4, 0);
		/* make reading non-blocking */
		fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK);
		/* read remaining */
		if (retrieved >= 0)
			retrieved += recv(sock, &buffer[retrieved], count - retrieved, 0);
	} else {
		/* non-blocking reading requested.
		 * this means that we expected more data.
		 * sleep a bit and read again */
		for (int a = 0; retrieved <= 0 && a < TELNET_NON_BLOCKING_ATTEMPTS; ++a) {
			*debugfile << TELNET_DEBUG_NAME << "Attempting to read more data" << endl;
			usleep(TELNET_NON_BLOCKING_DELAY);
			retrieved = recv(sock, buffer, count, 0);
		}
	}
	if (retrieved != 0 && retrieved % 1448 == 0) {
		/* if we get packets of size 1448 we probably didn't get it all.
		 * since we can't be sure, we'll have to read non-blocking */
		*debugfile << TELNET_DEBUG_NAME << "Received " << retrieved << " bytes, expecting more data" << endl;
		retrieved += retrieve(&buffer[retrieved], count - retrieved, false);
	}
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
	char buffer[TELNET_BUFFER_SIZE];
	transmit("l");
	int size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit(username);
	size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit(password);
	size = retrieve(buffer, TELNET_BUFFER_SIZE);
	transmit("p");
	size = retrieve(buffer, TELNET_BUFFER_SIZE);
}

void Telnet::stop() {
}

/* private methods */
int Telnet::transmit(const char *data, int length) {
	return send(sock, data, length, 0);
}
