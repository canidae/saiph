#ifndef _SCIMITARDATA_H
#define	_SCIMITARDATA_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Scimitar : public Weapon {
	public:
		static std::map<std::string, Scimitar *> scimitars;

		Scimitar(const std::string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2);
		virtual ~Scimitar() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Scimitar *scimitar);

	private:
		static void create(const std::string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
