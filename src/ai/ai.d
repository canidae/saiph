module ai.ai;

class Ai {
	public:
		/* called before analyze(), path map can not be trusted. should be used for handling questions/menus and doing zero turn actions */
		void preAnalyze() {
		}

		/* will only be called when no question or menu is shown and we've not set any action in preAnalyze(), path map can be trusted */
		void analyze() {
		}
}
