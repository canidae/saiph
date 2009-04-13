#ifndef BEATITUDE_H
#define BEATITUDE_H

#include <string>
#include "Analyzer.h"

class Item;
class Request;
class Saiph;

namespace analyzer {
	class Beatitude : public Analyzer {
	public:
		Beatitude(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool check_beatitude;

		bool beatify(const Item &item);
	};
}
#endif
