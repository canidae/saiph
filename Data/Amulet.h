#ifndef _AMULETDATA_H
#define	_AMULETDATA_H

#include <map>
#include <string>
#include "Item.h"

namespace data {
	class Amulet : public Item {
	public:
		static std::map<std::string, Amulet *> amulets;
		const bool confer_effect;

		Amulet(const std::string &name, int cost, int weight, int material, unsigned long long properties, bool confer_effect);
		~Amulet() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Amulet *amulet);

	private:
		static void create(const std::string &name, int cost, int weight, int material, unsigned long long properties, bool confer_effect);
	};
}
#endif
