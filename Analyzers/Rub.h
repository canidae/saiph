#ifndef RUB_H
#define	RUB_H

#include <string>
#include "../Analyzer.h"
#include "../Saiph.h"

class Rub : public Analyzer {
	public:
		Rub(Saiph *saiph);
		void parseMessages(const std::string &messages);
		void analyze();

	private:
		Saiph *saiph;
		unsigned char magic_lamp_key;
		void lookForMagicLamp();
};
#endif
