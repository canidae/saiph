module ai.ai;

class Ai {
	public:
		/* will only be called when we're shown a question, neither map nor pathing can be trusted */
		void handleQuestion() {
		}

		/* will only be called when we're shown a menu, neither map nor pathing can be trusted */
		void handleMenu() {
		}

		/* will always be called unless we're shown a question or menu, map can be trusted, but not pathing */
		void handleMessages() {
		}

		/* will only be called when no question or menu is shown and we've not set any action, map and pathing can be trusted */
		void analyze() {
		}
}
