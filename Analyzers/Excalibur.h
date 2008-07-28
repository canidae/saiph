#ifndef EXCALIBUR_H
/* defines */
#define EXCALIBUR_H
/* long sword */
#define EXCALIBUR_LONG_SWORD "long sword"
/* priorities */
#define EXCALIBUR_DIP_PRIORITY 225

/* forward declare */
class Excalibur;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Excalibur : public Analyzer {
	public:
		/* constructors */
		Excalibur(Saiph *saiph);

		/* methods */
		void complete();
		void finish();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		bool excalibur_exists;
		string command2;
};
#endif
