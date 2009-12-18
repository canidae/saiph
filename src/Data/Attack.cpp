#include "Data/Attack.h"

using namespace data;

/* public constructors */
Attack::Attack(int type, int damage_type, int dices, int sides) : _type(type), _damage_type(damage_type), _dices(dices), _sides(sides), _min_damage(_dices), _avg_damage((_dices * _sides + _dices) / 2), _max_damage(_dices * _sides) {
}

/* destructor */
Attack::~Attack() {
}

/* public methods */
int Attack::type() const {
	return _type;
}

int Attack::damageType() const {
	return _damage_type;
}

int Attack::dices() const {
	return _dices;
}

int Attack::sides() const {
	return _sides;
}

int Attack::minDamage() const {
	return _min_damage;
}

int Attack::avgDamage() const {
	return _avg_damage;
}

int Attack::maxDamage() const {
	return _max_damage;
}

