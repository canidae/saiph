module ai.ai;

import ai.behavioural;
import action;

class Ai {
	/* called when a question not answered by an action appears, when this happens then no other methods are called */
	abstract Action handleQuestion();

	/* called when a menu not answered by an action appears, when this happens then no other methods are called */
	abstract Action handleMenu();

	/* called after new frame is drawn, should only be used for updating World/Saiph/Level/Tiles and not decision making */
	abstract void parseFrame();

	/* called before path map is updated, can be used for decision making that doesn't require pathing */
	abstract Action preAnalyze();

	/* called after path map is updated, typically used for decision making that requires pathing */
	abstract Action analyze();

	static Ai createAi() {
		return new Behavioural();
	}
}
