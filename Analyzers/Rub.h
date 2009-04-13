#ifndef RUB_H
#define	RUB_H

#include <string>
#include "Analyzer.h"
#include "../Request.h"

class Item;
class Saiph;

class Rub : public Analyzer {
	public:
		Rub(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		Request req;
		unsigned char magic_lamp_key;

		void findMagicLamp();
		bool isMagicLamp(const Item &item);
};
#endif
