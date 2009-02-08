#ifndef GENOCIDE_H
#define	GENOCIDE_H

#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Saiph.h"

class Genocide : public Analyzer {
	public:
		Genocide(Saiph *saiph);

		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		std::list<std::string> monsters;
		std::list<char> classes;
};
#endif
