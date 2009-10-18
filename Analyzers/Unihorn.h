#ifndef ANALYZER_UNIHORN_H
#define ANALYZER_UNIHORN_H

#include <string>
#include "Analyzer.h"
#include "../Request.h"

/* turns we'll have to wait before applying unihorn again after successful use */
#define UNIHORN_UNIHORN_TIMEOUT 5
/* messages */
#define UNIHORN_NOTHING_HAPPENS "  Nothing happens.  " // nothing can be fixed
#define UNIHORN_NOTHING_SEEMS_TO_HAPPEN "  Nothing seems to happen.  " // something can be fixed, but it wasn't

class Item;
class Saiph;

namespace analyzer {
	class Unihorn : public Analyzer {
	public:
		Unihorn(Saiph* saiph);

		void analyze();
		void parseMessages(const std::string& messages);
		bool request(const Request& request);

	private:
		Saiph* saiph;
		Request req;
		unsigned char unihorn_key;
		int apply_priority;
		int unihorn_use_turn;

		void findUnihorn();
		bool isUnihorn(const Item& item);
	};
}
#endif
