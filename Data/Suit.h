#ifndef DATA_SUIT_H
#define DATA_SUIT_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {

	class Suit : public Armor {
	public:
		static std::map<std::string, Suit*> suits;

		Suit(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		virtual ~Suit() {
		}

		static void init();

	protected:
		static void addToMap(const std::string& name, Suit* suit);

	private:
		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
