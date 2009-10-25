#ifndef DATA_WEAPON_H
#define DATA_WEAPON_H

#include <map>
#include <string>
#include <vector>
#include "Item.h"
#include "Attack.h"
#include "../Globals.h"

namespace data {

	class Weapon : public Item {
	public:
		virtual ~Weapon();

		static void init();
		static const std::map<const std::string, const Weapon*> weapons();
		const bool& oneHanded();
		const int& alignment();
		const std::vector<Attack>& attackSmall();
		const std::vector<Attack>& attackLarge();

	protected:
		Weapon(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Weapon* weapon);

	private:
		static std::map<const std::string, const Weapon*> _weapons;
		bool _one_handed;
		int _alignment;
		std::vector<Attack> _attack_small;
		std::vector<Attack> _attack_large;
	};
}
#endif
