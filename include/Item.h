#ifndef ITEM_H
#define ITEM_H

#include <string>

class Item {
public:
	Item(const std::string& text, int want = 0);
	Item();

	const std::string& name() const;
	const std::string& name(const std::string &name);
	int count() const;
	int count(int count);
	int beatitude() const;
	int beatitude(int beatitude);
	bool greased() const;
	bool greased(bool greased);
	bool fixed() const;
	bool fixed(bool fixed);
	int damage() const;
	int damage(int damage);
	bool unknownEnchantment() const;
	bool unknownEnchantment(bool unknown_enchantment);
	int enchantment() const;
	int enchantment(int enchantment);
	const std::string& additional() const;
	const std::string& additional(const std::string& additional);
	int want() const;
	int want(int want);

	bool operator==(const Item& i) const;
	bool operator!=(const Item& i) const;

private:
	std::string _name;
	int _count;
	int _beatitude;
	bool _greased;
	bool _fixed;
	int _damage;
	bool _unknown_enchantment;
	int _enchantment;
	std::string _additional;
	int _want; // this is used by analyzers and event::WantItem to determine how many we want of this item when looting/dropping
	bool _is_item; // used for speeding up comparison;
};

std::ostream & operator<<(std::ostream& out, const Item& item);
#endif
