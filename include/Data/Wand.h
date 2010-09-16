#ifndef DATA_WAND_H
#define DATA_WAND_H

#include <map>
#include <string>
#include "Data/Item.h"

namespace data {

	class Wand : public Item {
	public:
		virtual ~Wand();

		static void init();
		static const std::map<const std::string, const Wand*>& wands();

		int recharge();
		int charges();

	protected:
		Wand(const std::string& name, int cost, int weight, int material, unsigned long long properties);

		static void addToMap(const std::string& name, const Wand* wand);

	private:
		static std::map<const std::string, const Wand*> _wands;

		static void create(const std::string& name, int cost, int weight, int material, unsigned long long properties);

		int _recharge;
		int _charges;
	};
}
#endif
