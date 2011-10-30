#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "Debug.h"
#include "Globals.h"
#include "Telnet.h"
#include "World.h"

using namespace std;

/* constructors/destructor */
Telnet::Telnet() : _vt_tiledata2011(false), _overs_pending(0) {
	/* read login details from .account */
	ifstream account;
	account.open(".account");
	if (!account) {
		Debug::error() << "Unable to read .account (which should contain url, username and password on separate lines)" << endl;
		World::destroy();
		exit(1);
	}
	string url;
	string username;
	string password;
	account >> url;
	account >> username;
	account >> password;
	account.close();
	username.append("\n");
	password.append("\n");

	/* connect to host using telnet */
	struct hostent* he = gethostbyname(url.c_str());
	if (he == NULL) {
		herror("gethostbyname");
		World::destroy();
		exit(1);
	}
	_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (_sock == -1) {
		perror("socket");
		World::destroy();
		exit(1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(TELNET_NETHACK_PORT);
	addr.sin_addr = *((struct in_addr*) he->h_addr);
	memset(addr.sin_zero, '\0', sizeof (addr.sin_zero));

	if (connect(_sock, (struct sockaddr*) &addr, sizeof (addr)) == -1) {
		perror("connect");
		World::destroy();
		exit(1);
	}

	/* set up ping */
	_ping[0] = 0xff; // IAC
	_ping[1] = 0xfd; // DO
	_ping[2] = 0x63; // decimal 99, which is our "ping"

	/* tell server our terminal (xterm-color) and size (80x24) and set some other things */
	char data[] = {0xff, 0xfb, 0x18, 0xff, 0xfa, 0x18, 0x00, 'x', 't', 'e', 'r', 'm', 0xff, 0xf0, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x23, 0xff, 0xfc, 0x27, 0xff, 0xfe, 0x03, 0xff, 0xfb, 0x01, 0xff, 0xfd, 0x05, 0xff, 0xfb, 0x21, 0xff, 0xfb, 0x1f, 0xff, 0xfa, 0x1f, 0x00, 0x50, 0x00, 0x18, 0xff, 0xf0};
	transmit(data, 47);

	/* that last command we sent to will send us data back.
	 * let's discard it */
	char discard[4096];
	doRetrieve(discard, 4096);

	/* and let's log in */
	transmit("l");
	doRetrieve(discard, TELNET_BUFFER_SIZE);
	transmit(username);
	doRetrieve(discard, TELNET_BUFFER_SIZE);
	transmit(password);
	doRetrieve(discard, TELNET_BUFFER_SIZE);
	/* 19th december 2008 sporkhack was added to nao,
	 * giving us one more menu.
	 * send "1" for nethack and hope it'll still work
	 * on other servers */
	/* and about a year later it was changed so no need to send "1" anymore */
	//transmit("1");
	//doRetrieve(discard, TELNET_BUFFER_SIZE);

	/* and start a game */
	start();
}

/* destructor */
Telnet::~Telnet() {
	stop();
	close(_sock);
}

/* methods */
int Telnet::doRetrieve(char* buffer, int count) {
	int returned = -1;
	int retrieved = 0;
	int overs = 0;
	/* if the target is cooperating, we don't need to play games with the telnetd */
	if (_vt_tiledata2011) {
		while ((overs = scanOvers(buffer, retrieved)) < _overs_pending) {
			returned = recv(_sock, &buffer[retrieved], count - retrieved - 1, 0);
			retrieved += returned;
		}
		_overs_pending -= overs;
		retrieved = removeOvers(buffer, retrieved);
		buffer[retrieved] = 0;
		return retrieved;
	}
	/* this is borrowed from TAEB:
	 * we can send a "ping" by transmitting [0xff, 0xfd, 0x63].
	 * then we'll just read until last bytes equal [?, ?, ?] */
	transmit(_ping, 3);
	/* sleep a bit in case of very low latency */
	usleep(50000);
	int more_data = true;
	int tries = 5;
	while (more_data && tries >= 0 && returned != 0) {
		returned = recv(_sock, &buffer[retrieved], count, 0);
		retrieved += returned;
		for (int a = retrieved - 3; a >= 0; --a) {
			/* search for pong backwards (we're expecting pong to be last).
			 * we do it like this because if we find it somewhere else than
			 * at the end of the stream, we should send a new ping */
			if ((unsigned char) buffer[a] == 0xff && (unsigned char) buffer[a + 1] == 0xfc && (unsigned char) buffer[a + 2] == 0x63) {
				/* found pong, but is it at end of stream? */
				if (a == retrieved - 3) {
					/* yes, it is.
					 * however, did we receive something else than the pong? */
					if (retrieved > 3) {
						/* yes, we did, good! */
						more_data = false;
						/* remove pong */
						buffer[--retrieved] = '\0';
						buffer[--retrieved] = '\0';
						buffer[--retrieved] = '\0';
					} else {
						/* no, we didn't. hmmm */
						retrieved = 0;
						transmit(_ping, 3);
						--tries; // also decrease this, in case we're not getting any data */
					}
				} else {
					/* no, it isn't.
					 * we should send a new ping */
					transmit(_ping, 3);
					/* and we'll have to remove the pong reply, gah */
					for (int b = a; b < retrieved - 3; ++b)
						buffer[b] = buffer[b + 3];
					retrieved -= 3;
					break;
				}
			}
		}
	}
	overs = scanOvers(buffer, retrieved);
	if (overs) {
		// good news!  we just started a nethack with vt_tiledata 2011 enabled.
		// we don't need to use any of this ping/pong hacks anymore
		_vt_tiledata2011 = true;
		_overs_pending -= overs;
		retrieved = removeOvers(buffer, retrieved);
		Debug::info() << "Detected vt_tiledata 2011.  Switching to synchronous mode." << endl;
	}
	buffer[retrieved] = 0;
	if (tries < 0)
		Debug::warning() << "We were expecting data, but got none. Fix this bug, please!" << endl;
	return retrieved;
}

int Telnet::transmit(const string& data) {
	_overs_pending += data.length();
	return transmit(data.c_str(), data.length());
}

void Telnet::start() {
	transmit("p");
	// previous character(s) will not be seen by nethack
	_overs_pending = 1;
}

void Telnet::stop() {
}

/* private methods */
/* doesn't count towards _overs_pending because this is used for sending the ping */
int Telnet::transmit(const char* data, int length) {
	return send(_sock, data, length, 0);
}

int Telnet::scanOvers(const char* data, int length) {
	int found = 0;
	for (const char* ptr = data; ptr <= data + length - 4; ++ptr) {
		if (ptr[0] == 27 && ptr[1] == '[' && ptr[2] == '3' && ptr[3] == 'z') {
			found++;
			ptr += 3;
		}
	}
	return found;
}

// XXX: the core expects to find --More-- at the end of data, so we need to remove ^[[3z from the data.
int Telnet::removeOvers(char *data, int length) {
	char* rp = data;
	char* wp = data;
	while (rp <= data + length - 4) {
		if (rp[0] == 27 && rp[1] == '[' && rp[2] == '3' && rp[3] == 'z') {
			rp += 4;
		} else {
			*(wp++) = *(rp++);
		}
	}
	while (rp != data + length)
		*(wp++) = *(rp++);
	return wp - data;
}
