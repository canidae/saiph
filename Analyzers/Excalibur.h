#ifndef EXCALIBUR_H
#define EXCALIBUR_H
/* long sword */
#define EXCALIBUR_LONG_SWORD "long sword"

#include <string>
#include "Analyzer.h"
#include "../Request.h"

class Saiph;

class Excalibur : public Analyzer {
	public:
		Excalibur(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Request req;
		std::string command2;
};
#endif
