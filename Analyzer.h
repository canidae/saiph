#ifndef ANALYZER_H
/* defines */
#define ANALYZER_H
/* max symbols an analyzer can track */
#define MAX_SYMBOLS 64
/* various ambigous messages */
#define CHOOSE_DIRECTION "In what direction?"
#define FOOT_HURT "Your right leg is in no shape for kicking."
#define FOOT_UNHURT "Your leg feels somewhat better."

/* forward declare */
class Analyzer;

/* includes */
#include <string>

/* namespace */
using namespace std;

/* analyze every tile of type and/or do whatever you want in finish() */
class Analyzer {
	public:
		/* variables */
		int max_priority;
		char symbols[MAX_SYMBOLS];
		int symbol_count;

		/* constructors */
		Analyzer();

		/* destructors */
		virtual ~Analyzer();

		/* methods */
		virtual int parseMessages(string *messages);
		virtual int start();
		virtual int analyze(int row, int col, char symbol);
		virtual int finish();
		virtual void command();
};
#endif
