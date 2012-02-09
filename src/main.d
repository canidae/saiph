import data.point;
import io.connection;
import io.telnet;

void main() {
	Connection conn = new Telnet();
	conn.connect();
	/* loop here */
	bool active = true;
	while (active) {
		active = conn.update();
		//conn.send(cast(char[]) "TODO");
	}
	conn.disconnect();
}
