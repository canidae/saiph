#ifndef DATA_LANCE_H
#define DATA_LANCE_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Lance : public Weapon {
	public:
		virtual ~Lance();

		static void init();
		static const std::map<const std::string, const Lance*>& lances();

	protected:
		Lance(const std::string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Lance* lance);

	private:
		static std::map<const std::string, const Lance*> _lances;

		static void create(const std::string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
