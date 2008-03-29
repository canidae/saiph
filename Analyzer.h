#ifndef ANALYZER_H
#define ANALYZER_H

/* forward declare */
class Analyzer;

/* defines */
#define ANALYZE_NONE 0x0000
#define ANALYZE_DOOR 0x0001
#define ANALYZE_MONSTER 0x0002
#define ANALYZE_OBJECT 0x0004
#define ANALYZE_PASSABLE 0x0008
#define ANALYZE_TRAP 0x0010

using namespace std;

/* an analyzer can do whatever it wants */
class Analyzer {
	public:
		/* variables */
		int type;

		/* constructors */
		Analyzer();

		/* destructors */
		virtual ~Analyzer();

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();
};

#endif
