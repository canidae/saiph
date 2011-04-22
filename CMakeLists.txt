cmake_minimum_required(VERSION 2.6)
project(SAIPH)

if (CMAKE_BUILD_TYPE STREQUAL "")
        set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: None, Debug or Release." FORCE)
endif ()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -s")

find_library(INETUTILS_LIBRARIES NAMES util)

include_directories(
	${SAIPH_SOURCE_DIR}/include
)

set(HEADER_FILES
	# Main
	include/Command.h
	include/Connection.h
	include/Coordinate.h
	include/Debug.h
	include/EventBus.h
	include/Globals.h
	include/Inventory.h
	include/Item.h
	include/Level.h
	include/Local.h
	include/Monster.h
	include/Point.h
	include/Replay.h
	include/Saiph.h
	include/Stash.h
	include/Telnet.h
	include/Tile.h
	include/World.h
	# Actions
	include/Actions/Action.h
	include/Actions/Answer.h
	include/Actions/Apply.h
	include/Actions/ApplyInDirection.h
	include/Actions/Call.h
	include/Actions/Donate.h
	include/Actions/DropGold.h
	include/Actions/Drop.h
	include/Actions/EatCorpse.h
	include/Actions/Eat.h
	include/Actions/Engrave.h
	include/Actions/Enhance.h
	include/Actions/FarLook.h
	include/Actions/Fight.h
	include/Actions/Kick.h
	include/Actions/ListInventory.h
	include/Actions/ListPlayerAttributes.h
	include/Actions/Look.h
	include/Actions/Loot.h
	include/Actions/Move.h
	include/Actions/Name.h
	include/Actions/Open.h
	include/Actions/Pray.h
	include/Actions/PutOn.h
	include/Actions/Remove.h
	include/Actions/Rest.h
	include/Actions/Search.h
	include/Actions/TakeOff.h
	include/Actions/Throw.h
	include/Actions/Unlock.h
	include/Actions/Wear.h
	include/Actions/Wield.h
	# Analyzers
	include/Analyzers/Amulet.h
	include/Analyzers/Analyzer.h
	include/Analyzers/Armor.h
	include/Analyzers/Beatitude.h
	include/Analyzers/Boulder.h
#	include/Analyzers/Blind.h
#	include/Analyzers/Dig.h
	include/Analyzers/DiggingTool.h
	include/Analyzers/Donate.h
	include/Analyzers/Door.h
	include/Analyzers/Elbereth.h
	include/Analyzers/Enhance.h
	include/Analyzers/Excalibur.h
	include/Analyzers/Explore.h
	include/Analyzers/Fight.h
	include/Analyzers/Food.h
#	include/Analyzers/Genocide.h
	include/Analyzers/Health.h
	include/Analyzers/Lamp.h
	include/Analyzers/Loot.h
	include/Analyzers/MonsterInfo.h
#	include/Analyzers/Passtune.h
#	include/Analyzers/Potion.h
#	include/Analyzers/Ring.h
#	include/Analyzers/Rub.h
#	include/Analyzers/Scroll.h
	include/Analyzers/Quest.h
	include/Analyzers/Shop.h
	include/Analyzers/Sokoban.h
#	include/Analyzers/Throne.h
	include/Analyzers/Vault.h
	include/Analyzers/Wand.h
	include/Analyzers/Weapon.h
	include/Analyzers/Wish.h
	# Data
	include/Data/Amulet.h
	include/Data/Armor.h
	include/Data/Attack.h
	include/Data/Axe.h
	include/Data/Boomerang.h
	include/Data/Boots.h
	include/Data/BowAmmo.h
	include/Data/Bow.h
	include/Data/Broadsword.h
	include/Data/Cloak.h
	include/Data/Club.h
	include/Data/Corpse.h
	include/Data/CrossbowAmmo.h
	include/Data/Crossbow.h
	include/Data/Dagger.h
	include/Data/Dart.h
	include/Data/Flail.h
	include/Data/Food.h
	include/Data/Gloves.h
	include/Data/Hammer.h
	include/Data/Helmet.h
	include/Data/Item.h
	include/Data/Javelin.h
	include/Data/Key.h
	include/Data/Knife.h
	include/Data/Lamp.h
	include/Data/Lance.h
#	include/Data/Level.h
	include/Data/Longsword.h
	include/Data/Mace.h
	include/Data/Monster.h
	include/Data/MorningStar.h
	include/Data/Pickaxe.h
	include/Data/Polearm.h
	include/Data/Quarterstaff.h
	include/Data/Saber.h
	include/Data/Scimitar.h
	include/Data/Shield.h
	include/Data/Shirt.h
	include/Data/Shortsword.h
	include/Data/Shuriken.h
	include/Data/Skill.h
	include/Data/SlingAmmo.h
	include/Data/Sling.h
	include/Data/Spear.h
	include/Data/Suit.h
	include/Data/Tool.h
	include/Data/Trident.h
	include/Data/TwoHandedSword.h
	include/Data/UnicornHorn.h
	include/Data/Wand.h
	include/Data/Weapon.h
	include/Data/Whip.h
	# Events
	include/Events/Beatify.h
	include/Events/ChangedInventoryItems.h
	include/Events/EatItem.h
	include/Events/ElberethQuery.h
	include/Events/Event.h
	include/Events/GotDiggingTool.h
	include/Events/ItemsOnGround.h
	include/Events/QuestStatus.h
	include/Events/ReceivedItems.h
	include/Events/ShopDetected.h
	include/Events/StashChanged.h
	include/Events/WantItems.h
)

set(SOURCE_FILES
	# Main
	src/Command.cpp
	src/Connection.cpp
	src/Coordinate.cpp
	src/Debug.cpp
	src/EventBus.cpp
	src/Inventory.cpp
	src/Item.cpp
	src/Level.cpp
	src/Local.cpp
	src/Monster.cpp
	src/Point.cpp
	src/Saiph.cpp
	src/Stash.cpp
	src/Replay.cpp
	src/Telnet.cpp
	src/Tile.cpp
	src/World.cpp
	# Actions
	src/Actions/Action.cpp
	src/Actions/Answer.cpp
	src/Actions/Apply.cpp
	src/Actions/ApplyInDirection.cpp
	src/Actions/Call.cpp
	src/Actions/Dip.cpp
	src/Actions/Donate.cpp
	src/Actions/Drop.cpp
	src/Actions/DropGold.cpp
	src/Actions/Eat.cpp
	src/Actions/EatCorpse.cpp
	src/Actions/Engrave.cpp
	src/Actions/Enhance.cpp
	src/Actions/FarLook.cpp
	src/Actions/Fight.cpp
	src/Actions/Kick.cpp
	src/Actions/ListInventory.cpp
	src/Actions/ListPlayerAttributes.cpp
	src/Actions/Look.cpp
	src/Actions/Loot.cpp
	src/Actions/Move.cpp
	src/Actions/Name.cpp
	src/Actions/Open.cpp
	src/Actions/Pay.cpp
	src/Actions/Pray.cpp
	src/Actions/PutOn.cpp
	src/Actions/Remove.cpp
	src/Actions/Rest.cpp
	src/Actions/Search.cpp
	src/Actions/TakeOff.cpp
	src/Actions/Throw.cpp
	src/Actions/Unlock.cpp
	src/Actions/Wear.cpp
	src/Actions/Wield.cpp
	# Analyzers
	src/Analyzers/Amulet.cpp
	src/Analyzers/Analyzer.cpp
	src/Analyzers/Armor.cpp
	src/Analyzers/Beatitude.cpp
	src/Analyzers/Boulder.cpp
#	src/Analyzers/Blind.cpp
#	src/Analyzers/Dig.cpp
	src/Analyzers/DiggingTool.cpp
	src/Analyzers/Donate.cpp
	src/Analyzers/Door.cpp
	src/Analyzers/Elbereth.cpp
	src/Analyzers/Enhance.cpp
	src/Analyzers/Excalibur.cpp
	src/Analyzers/Explore.cpp
	src/Analyzers/Fight.cpp
	src/Analyzers/Food.cpp
#	src/Analyzers/Genocide.cpp
	src/Analyzers/Health.cpp
	src/Analyzers/Lamp.cpp
	src/Analyzers/Loot.cpp
	src/Analyzers/MonsterInfo.cpp
#	src/Analyzers/Passtune.cpp
#	src/Analyzers/Potion.cpp
#	src/Analyzers/Ring.cpp
#	src/Analyzers/Rub.cpp
#	src/Analyzers/Scroll.cpp
	src/Analyzers/Quest.cpp
	src/Analyzers/Shop.cpp
	src/Analyzers/Sokoban.cpp
#	src/Analyzers/Throne.cpp
	src/Analyzers/Vault.cpp
	src/Analyzers/Wand.cpp
	src/Analyzers/Weapon.cpp
	src/Analyzers/Wish.cpp
	# Data
	src/Data/Amulet.cpp
	src/Data/Armor.cpp
	src/Data/Attack.cpp
	src/Data/Axe.cpp
	src/Data/Boomerang.cpp
	src/Data/Boots.cpp
	src/Data/BowAmmo.cpp
	src/Data/Bow.cpp
	src/Data/Broadsword.cpp
	src/Data/Cloak.cpp
	src/Data/Club.cpp
	src/Data/Corpse.cpp
	src/Data/CrossbowAmmo.cpp
	src/Data/Crossbow.cpp
	src/Data/Dagger.cpp
	src/Data/Dart.cpp
	src/Data/Flail.cpp
	src/Data/Food.cpp
	src/Data/Gloves.cpp
	src/Data/Hammer.cpp
	src/Data/Helmet.cpp
	src/Data/Item.cpp
	src/Data/Javelin.cpp
	src/Data/Key.cpp
	src/Data/Knife.cpp
	src/Data/Lamp.cpp
	src/Data/Lance.cpp
#	src/Data/Level.cpp
	src/Data/Longsword.cpp
	src/Data/Mace.cpp
	src/Data/Monster.cpp
	src/Data/MorningStar.cpp
	src/Data/Pickaxe.cpp
	src/Data/Polearm.cpp
	src/Data/Quarterstaff.cpp
	src/Data/Saber.cpp
	src/Data/Scimitar.cpp
	src/Data/Shield.cpp
	src/Data/Shirt.cpp
	src/Data/Shortsword.cpp
	src/Data/Shuriken.cpp
	src/Data/Skill.cpp
	src/Data/SlingAmmo.cpp
	src/Data/Sling.cpp
	src/Data/Spear.cpp
	src/Data/Suit.cpp
	src/Data/Tool.cpp
	src/Data/Trident.cpp
	src/Data/TwoHandedSword.cpp
	src/Data/UnicornHorn.cpp
	src/Data/Wand.cpp
	src/Data/Weapon.cpp
	src/Data/Whip.cpp
	# Events
	src/Events/Event.cpp
)

add_executable(saiph ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(saiph ${INETUTILS_LIBRARIES})
