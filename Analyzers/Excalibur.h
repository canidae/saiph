#ifndef EXCALIBUR_H
#define EXCALIBUR_H
/* long sword */
#define EXCALIBUR_LONG_SWORD "long sword"
/* priorities */
#define EXCALIBUR_DIP_PRIORITY 225

#include <string>
#include "../Analyzer.h"

class Saiph;

class Excalibur : public Analyzer {
	public:
		Excalibur(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool excalibur_exists;
		std::string command2;
};
#endif
