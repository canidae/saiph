import data.point;
import io.connection;
import io.telnet;

void main() {
	Connection conn = new Telnet();
	//conn.connect();
	/* loop here */
	while (false) {
		conn.update(cast(char[]) "TODO");
	}
	//conn.disconnect();
}
