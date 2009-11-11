#ifndef DATA_KNIFE_H
#define DATA_KNIFE_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Knife : public Weapon {
	public:
		virtual ~Knife();

		static void init();
		static const std::map<const std::string, const Knife*>& knives();

	protected:
		Knife(const std::string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Knife* knife);

	private:
		static std::map<const std::string, const Knife*> _knives;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
