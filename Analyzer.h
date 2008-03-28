#ifndef ANALYZER_H
#define ANALYZER_H

/* forward declare */
class Analyzer;

using namespace std;

/* an analyzer can do whatever it wants */
class Analyzer {
	public:
		/* variables */

		/* constructors */
		Analyzer();

		/* destructors */
		virtual ~Analyzer();

		/* methods */
		virtual void analyze() const = 0;
};

#endif
