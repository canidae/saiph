#ifndef ANALYZER_EXCALIBUR_H
#define ANALYZER_EXCALIBUR_H

#include <string>
#include "Analyzer.h"
#include "../Request.h"

/* long sword */
#define EXCALIBUR_LONG_SWORD "long sword"

class Saiph;

namespace analyzer {
	class Excalibur : public Analyzer {
	public:
		Excalibur(Saiph* saiph);

		void analyze();
		void parseMessages(const std::string& messages);

	private:
		Saiph* saiph;
		Request req;
		std::string command2;
	};
}
#endif
