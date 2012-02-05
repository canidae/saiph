module data.saiph;

import data.coordinate;

class Saiph {
public:
	@property static auto alignment() {
		return _alignment;
	}

	@property static auto alignment(int alignment) {
		return _alignment = alignment;
	}

	@property static auto charisma() {
		return _charisma;
	}

	@property static auto charisma(int charisma) {
		return _charisma = charisma;
	}

	@property static auto constitution() {
		return _constitution;
	}

	@property static auto constitution(int constitution) {
		return _constitution = constitution;
	}

	@property static auto dexterity() {
		return _dexterity;
	}

	@property static auto dexterity(int dexterity) {
		return _dexterity = dexterity;
	}

	@property static auto intelligence() {
		return _intelligence;
	}

	@property static auto intelligence(int intelligence) {
		return _intelligence = intelligence;
	}

	@property static auto strength() {
		return _strength;
	}

	@property static auto strength(int strength) {
		return _strength = strength;
	}

	@property static auto wisdom() {
		return _wisdom;
	}

	@property static auto wisdom(int wisdom) {
		return _wisdom = wisdom;
	}

	@property static auto armor() {
		return _armor;
	}

	@property static auto armor(int armor) {
		return _armor = armor;
	}

	@property static auto encumbrance() {
		return _encumbrance;
	}

	@property static auto encumbrance(int encumbrance) {
		return _encumbrance = encumbrance;
	}

	@property static auto experience() {
		return _experience;
	}

	@property static auto experience(int experience) {
		return _experience = experience;
	}

	@property static auto hunger() {
		return _hunger;
	}

	@property static auto hunger(int hunger) {
		return _hunger = hunger;
	}

	@property static auto hitpoints() {
		return _hitpoints;
	}

	@property static auto hitpoints(int hitpoints) {
		return _hitpoints = hitpoints;
	}

	@property static auto hitpointsMax() {
		return _hitpointsMax;
	}

	@property static auto hitpointsMax(int hitpointsMax) {
		return _hitpointsMax = hitpointsMax;
	}

	@property static auto power() {
		return _power;
	}

	@property static auto power(int power) {
		return _power = power;
	}

	@property static auto powerMax() {
		return _powerMax;
	}

	@property static auto powerMax(int powerMax) {
		return _powerMax = powerMax;
	}

	@property static auto blind() {
		return _blind;
	}

	@property static auto blind(bool blind) {
		return _blind = blind;
	}

	@property static auto confused() {
		return _confused;
	}

	@property static auto confused(bool confused) {
		return _confused = confused;
	}

	@property static auto poisoned() {
		return _poisoned;
	}

	@property static auto poisoned(bool poisoned) {
		return _poisoned = poisoned;
	}

	@property static auto hallucinating() {
		return _hallucinating;
	}

	@property static auto hallucinating(bool hallucinating) {
		return _hallucinating = hallucinating;
	}

	@property static auto ill() {
		return _ill;
	}

	@property static auto ill(bool ill) {
		return _ill = ill;
	}

	@property static auto slimed() {
		return _slimed;
	}

	@property static auto slimed(bool slimed) {
		return _slimed = slimed;
	}

	@property static auto stoned() {
		return _stoned;
	}

	@property static auto stoned(bool stoned) {
		return _stoned = stoned;
	}

	@property static auto stunned() {
		return _stunned;
	}

	@property static auto stunned(bool stunned) {
		return _stunned = stunned;
	}

	@property static auto hurtLeg() {
		return _hurtLeg;
	}

	@property static auto hurtLeg(bool hurtLeg) {
		return _hurtLeg = hurtLeg;
	}

	@property static auto polymorphed() {
		return _polymorphed;
	}

	@property static auto polymorphed(bool polymorphed) {
		return _polymorphed = polymorphed;
	}

	@property static auto engulfed() {
		return _engulfed;
	}

	@property static auto engulfed(bool engulfed) {
		return _engulfed = engulfed;
	}

	@property static auto inPit() {
		return _inPit;
	}

	@property static auto inPit(bool inPit) {
		return _inPit = inPit;
	}

	@property static auto position() {
		return _position;
	}

	@property static auto position(Coordinate position) {
		return _position = position;
	}

	@property static auto zorkmids() {
		return _zorkmids;
	}

	@property static auto zorkmids(int zorkmids) {
		return _zorkmids = zorkmids;
	}


private:
	/* attributes */
	static int _alignment;
	static int _charisma;
	static int _constitution;
	static int _dexterity;
	static int _intelligence;
	static int _strength;
	static int _wisdom;
	/* status */
	static int _armor;
	static int _encumbrance;
	static int _experience;
	static int _hunger;
	static int _hitpoints;
	static int _hitpointsMax;
	static int _power;
	static int _powerMax;
	/* effects */
	static bool _blind;
	static bool _confused;
	static bool _poisoned;
	static bool _hallucinating;
	static bool _ill;
	static bool _slimed;
	static bool _stoned;
	static bool _stunned;
	static bool _hurtLeg;
	static bool _polymorphed;
	static bool _engulfed;
	static bool _inPit;
	/* position */
	static Coordinate _position;
	/* zorkmids */
	static int _zorkmids;
}
