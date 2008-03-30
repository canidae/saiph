#ifndef ANALYZER_H
/* defines */
#define ANALYZER_H
/* tiles to be analyzed */
#define ANALYZE_NONE 0x0000
#define ANALYZE_CLOSED_DOOR 0x0001
#define ANALYZE_OPEN_DOOR 0x0002
#define ANALYZE_OBJECT 0x0004
#define ANALYZE_PASSABLE 0x0008
#define ANALYZE_UNPASSABLE 0x0010
#define ANALYZE_MONSTER 0x0020
#define ANALYZE_TRAP 0x0040
#define ANALYZE_UNEXPLORED 0x0080
#define ANALYZE_PLAYER 0x0100
#define ANALYZE_PET 0x0200
#define ANALYZE_BOULDER 0x0400
/* or analyze all tiles */
#define ANALYZE_ALL 0xffff

/* forward declare */
class Analyzer;

/* includes */

/* namespace */
using namespace std;

/* analyze every tile of type and/or do whatever you want in finish() */
class Analyzer {
	public:
		/* variables */
		int type;

		/* constructors */
		Analyzer();

		/* destructors */
		virtual ~Analyzer();

		/* methods */
		virtual void start();
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();
};

#endif
