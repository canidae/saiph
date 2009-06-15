#ifndef DATA_SHIRT_H
#define DATA_SHIRT_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {
	class Shirt : public Armor {
	public:
		static std::map<std::string, Shirt *> shirts;

		Shirt(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
		virtual ~Shirt() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Shirt *shirt);

	private:
		static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
