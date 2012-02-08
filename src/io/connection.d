module io.connection;

import data.point;

abstract class Connection {
public:
	abstract void connect();

	abstract void disconnect();

	void update(char[] data) {
		send(data);
		parse(receive());
	}

protected:
	abstract void send(char[] data);

	abstract char[] receive(char[] inputEndsWith = []);

private:
	Point cursor;
	int color;

	void parse(char[] data) {
		for (int i = 0; i < data.length; ++i) {
			char c = data[i];
			switch (c) {
				case 0, 14, 15:
					/* null characters and invoking character set commands we can ignore */
					break;

				case 8: 
					/* backspace, move cursor left (west) */
					cursor.moveWest();
					break;

				case 10:
					/* linefeed, go down (south) */
					cursor.moveSouth();
					break;

				case 27:
					/* escape sequence */
					i += parseEscapeSequence(data[++i .. $]);
					break;

				default:
					/* add character to map and move east */
					/* TODO */
					cursor.moveEast();
					break;

			}
		}
	}

	int parseEscapeSequence(char[] data) {
		int i = 0;
		char c = data[i];
		switch (c) {
			case 26, '=', '>':
				/* we can ignore <ESC> followed by <ESC>, '=' or '>' */
				break;

			case '(', ')', '*', '+':
				/* designate character set, ignore this and the following character */
				++i;
				break;

			case 'M':
				/* reverse linefeed, move up (north) */
				cursor.moveNorth();
				break;

			case '[':
				/* escape code, handle it */
				/* TODO */
				break;

			default:
				/* this is bad */
				/* TODO */
				break;
		}
		return i;
	}
}
