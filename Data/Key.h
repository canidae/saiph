#ifndef _KEYDATA_H
#define	_KEYDATA_H

#include <map>
#include <string>
#include "Tool.h"

namespace data {
	class Key : public Tool {
	public:
		static std::map<std::string, Key *> keys;

		Key(const std::string &name, int cost, int weight, int material, unsigned long long properties);
		~Key() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Key *key);

	private:
		static void create(const std::string &name, int cost, int weight, int material, unsigned long long properties);
	};
}
#endif
