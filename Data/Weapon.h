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
		static const std::map<const std::string, const Weapon*>& weapons();
		bool oneHanded() const;
		int alignment() const;
		int type() const;
		const std::vector<Attack>& attackSmall() const;
		const std::vector<Attack>& attackLarge() const;

	protected:
		Weapon(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, int type, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Weapon* weapon);

	private:
		static std::map<const std::string, const Weapon*> _weapons;
		bool _one_handed;
		int _alignment;
		int _type;
		std::vector<Attack> _attack_small;
		std::vector<Attack> _attack_large;
	};
}
#endif
