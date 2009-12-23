#ifndef DATA_WHIP_H
#define DATA_WHIP_H

#include <map>
#include <string>
#include "Data/Weapon.h"

namespace data {

	class Whip : public Weapon {
	public:
		virtual ~Whip();

		static void init();
		static const std::map<const std::string, const Whip*>& whips();

	protected:
		Whip(const std::string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Whip* whip);

	private:
		static std::map<const std::string, const Whip*> _whips;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
