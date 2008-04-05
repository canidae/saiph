#ifndef ANALYZER_H
/* defines */
#define ANALYZER_H
/* max symbols an analyzer can track */
#define MAX_SYMBOLS 64

/* forward declare */
class Analyzer;

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
		virtual int parseMessages();
		virtual int start();
		virtual int analyze(int row, int col, char symbol);
		virtual int finish();
		virtual void command();
};
#endif
