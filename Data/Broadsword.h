#ifndef DATA_BROADSWORD_H
#define DATA_BROADSWORD_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class Broadsword : public Weapon {
	public:
		virtual ~Broadsword();

		static void init();
		static const std::map<const std::string, const Broadsword*>& broadswords();

	protected:
		Broadsword(const std::string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, const bool& one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2);

		static void addToMap(const std::string& name, const Broadsword* broadsword);

	private:
		static std::map<const std::string, const Broadsword*> _broadswords;

		static void create(const std::string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, const bool& one_handed, const int& alignment);
	};
}
#endif
