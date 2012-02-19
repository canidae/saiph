import data.world;
import io.connection;
import io.telnet;

void main() {
	Connection conn = new Telnet();
	conn.connect();
	/* TODO: Ai ai = new MyAi(); */
	/* loop here */
	bool active = true;
	while (active) {
		active = conn.update();
		/* TODO: if in the middle of an action, continue that, else: */
		{
			/* TODO: ai.preAnalyze(); */
			if (!World.menu && ! World.question /* && no action set */) {
				/* TODO: update path map */
				/* TODO: ai.analyze(); */
			}
		}
		conn.send(cast(char[]) "#quit\ny q");
	}
	conn.disconnect();
}
