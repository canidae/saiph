#include "Player.h"

/* constructors */
Player::Player() {
	/* zero attributes */
	this->attributes.strength = 0;
	this->attributes.dexterity = 0;
	this->attributes.constitution = 0;
	this->attributes.intelligence = 0;
	this->attributes.wisdom = 0;
	this->attributes.charisma = 0;
	this->attributes.alignment = 0;

	/* zero status */
	this->status.dungeon = 0;
	this->status.zorkmids = 0;
	this->status.hitpoints = 0;
	this->status.hitpoints_max = 0;
	this->status.power = 0;
	this->status.power_max = 0;
	this->status.armor_class = 0;
	this->status.experience = 0;
	this->status.turn = 0;
	this->status.encumbrance = 0;
	this->status.hunger = 0;
	this->status.blind = false;
	this->status.confused = false;
	this->status.foodpoisoned = false;
	this->status.hallucinating = false;
	this->status.ill = false;
	this->status.slimed = false;
	this->status.stunned = false;

	/* zero position */
	this->position.col = 0;
	this->position.row = 0;
}

/* destructors */
Player::~Player() {
}

/* methods */
