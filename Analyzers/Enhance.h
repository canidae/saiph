#ifndef ENHANCE_H
#define ENHANCE_H

#include <string>
#include "../Analyzer.h"

class Saiph;

class Enhance : public Analyzer {
	public:
		Enhance(Saiph *saiph);

		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
};
#endif
