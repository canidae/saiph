module ai.behavioural;

import ai.ai;
import action;

class Behavioural : Ai {
	override Action handleQuestion() {
		return new Answer("\x27");
	}

	override Action handleMenu() {
		return new Answer("\x27");
	}

	override void parseFrame() {
	}

	override Action preAnalyze() {
		return null;
	}

	override Action analyze() {
		return null;
	}
}
