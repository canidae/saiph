import data.point;
import io.connection;
import io.telnet;

void main() {
	Connection conn = new Telnet();
	//conn.connect();
	/* loop here */
	while (false) {
		parse(conn.receive());
	}
	//conn.disconnect();
}

private:
Point _cursor;

@property auto cursor() {
	return _cursor;
}

@property auto cursor(Point cursor) {
	return _cursor = cursor;
}

void parse(char[] data) {
	for (int i = 0; i < data.length; ++i) {
		char c = data[i];
		switch (c) {
		case 0, 14, 15:
			/* null characters and invoking character sets we can ignore */
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
			c = data[++i];
			switch (c) {
			case 27, '=', '>':
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
			break;

		default:
			/* add character to map and move east */
			/* TODO */
			cursor.moveEast();
			break;

		}
	}
}
