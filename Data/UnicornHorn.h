#ifndef DATA_UNICORN_HORN_H
#define DATA_UNICORN_HORN_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class UnicornHorn : public Weapon {
	public:
		virtual ~UnicornHorn();

		static void init();
		static const std::map<const std::string, const UnicornHorn*>& unicornHorns();

	protected:
		UnicornHorn(const std::string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const UnicornHorn* unicorn_horn);

	private:
		static std::map<const std::string, const UnicornHorn*> _unicorn_horns;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
