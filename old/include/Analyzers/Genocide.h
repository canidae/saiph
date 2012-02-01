#ifndef ANALYZER_GENOCIDE_H
#define ANALYZER_GENOCIDE_H

#include <list>
#include <string>
#include "Saiph.h"
#include "Analyzers/Analyzer.h"

namespace analyzer {
	class Genocide : public Analyzer {
	public:
		Genocide(Saiph* saiph);

		void parseMessages(const std::string& messages);

	private:
		Saiph* saiph;
		std::list<std::string> monsters;
		std::list<char> classes;
	};
}
#endif
