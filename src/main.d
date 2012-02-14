import data.world;
import io.connection;
import io.telnet;

void main() {
	Connection conn = new Telnet();
	conn.connect();
	/* loop here */
	bool active = true;
	while (active) {
		active = conn.update();
		World.updateMap();
		//conn.send(cast(char[]) "TODO");
	}
	conn.disconnect();
}
