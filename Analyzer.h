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

		/* constructors */
		Analyzer();

		/* destructors */
		virtual ~Analyzer();

		/* methods */
		virtual void analyze(int row, int col, unsigned char symbol);
		virtual void command(string *command);
		virtual int finish();
		virtual int parseMessages(string *messages);
		virtual int start();
};
#endif
