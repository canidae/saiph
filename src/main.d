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
			if (World.menu) {
				/* TODO: ai.handleMenu() */
			} else {
				if (World.question) {
					/* TODO: ai.handleQuestion() */
				} else {
					/* TODO: ai.handleMessages(); */
					/* TODO: if no action after ai.handleMessages(): */
					{
						/* only update map when we're not showing a menu or a question */
						World.updateMap();
						/* TODO: ai.analyze(); */
					}
				}
			}
		}
		conn.send(cast(char[]) "#quit\ny q");
	}
	conn.disconnect();
}
