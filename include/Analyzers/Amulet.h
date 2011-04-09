#ifndef ANALYZER_AMULET_H
#define ANALYZER_AMULET_H

#include <set>
#include <string>
#include "Analyzers/Analyzer.h"

/* priorities */
#define PRIORITY_AMULET_LOOT 200
#define PRIORITY_AMULET_WEAR 200
#define PRIORITY_AMULET_REMOVE_HARM 925
/* bad amulet messages */
#define STRANGULATION_5TTL "  It constricts your throat!  "
// we don't use these...
#ifdef EXTRA_STRANGULATION
#define STRANGULATION_4TTL "  You find it hard to breathe.  "
#define STRANGULATION_3TTL "  You're gasping for air.  "
#define STRANGULATION_2TTL "  You can no longer breathe.  " 
#define STRANGULATION_1TTL "  You're turning blue.  " 
#endif
class Item;

namespace analyzer {

	class Amulet : public Analyzer {
	public:
		Amulet();
		
		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		unsigned char _amulet_key;

		bool wantItem(const Item& item);
		void wearAmulet(const std::set<unsigned char>& keys);
	};
}
#endif
