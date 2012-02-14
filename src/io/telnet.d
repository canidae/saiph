module io.telnet;

import io.connection;
import io.log;
import std.file;
import std.socket;
import std.stdio;
import std.string;

class Telnet : Connection {
public:
	void connect() {
		string[] lines = splitLines(readText(".account"));
		socket = new TcpSocket(new InternetAddress(lines[0], 23));
		/* setup terminal */
		send([0xff, 0xfb, 0x18, 0xff, 0xfa, 0x18, 0x00, 'x', 't', 'e', 'r', 'm', 0xff, 0xf0, 0xff, 0xfc, 0x20, 0xff, 0xfc, 0x23, 0xff, 0xfc, 0x27, 0xff, 0xfe, 0x03, 0xff, 0xfb, 0x01, 0xff, 0xfd, 0x05, 0xff, 0xfb, 0x21, 0xff, 0xfb, 0x1f, 0xff, 0xfa, 0x1f, 0x00, 0x50, 0x00, 0x18, 0xff, 0xf0]);
		/* wait for menu */
		auto data = receive(cast(char[]) "[19;3H=> ");
		/* login */
		send(cast(char[]) "l");
		data = receive(cast(char[]) "[8d => ");
		/* username */
		send(cast(char[]) lines[1] ~ "\n");
		data = receive(cast(char[]) "[8d => ");
		/* password (not using send() for this to prevent password from being logged) */
		log.info("Sending password    : ******");
		socket.send(cast(char[]) lines[2] ~ "\n");
		data = receive(cast(char[]) "[22;3H=> ");
		/* start game */
		send(cast(char[]) "p");
	}

	void disconnect() {
		socket.close();
	}

	void send(char[] data) {
		log.info("Sending %6s bytes: %s", data.length, data);
		socket.send(data);
	}

protected:
	char[] receive(char[] inputEndsWith = [0x1b, '[', '3', 'z']) {
		/* normally we expect data to end with "<ESC>[3z" (sent from NAO if option "vt_tiledata" is set) */
		char[32768] data; // TODO: we may suddenly get more data than this (frozen by floating eye cause lots of output), need to handle it better
		auto count = socket.receive(data);
		while (inputEndsWith.length > 0 && (count < inputEndsWith.length || data[count - inputEndsWith.length .. count] != inputEndsWith))
			count += socket.receive(data[count .. $]);
		log.info("Received %5s bytes: %s", count, data[0 .. count]);
		return data[0 .. count];
	}

private:
	static Logger log;
	Socket socket;

	static this() {
		log = new Logger("telnet");
	}
}
