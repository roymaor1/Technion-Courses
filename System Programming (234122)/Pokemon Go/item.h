//
// Created by Roy on 22/01/2017.
//

#ifndef POKEMON_ITEM_H
#define POKEMON_ITEM_H


#include "exceptions.h"

namespace mtm {
	namespace pokemongo {

		// Item types in game.
		typedef enum {
			CANDY,
			POTION,
		} ItemType;

		class Item {
			const ItemType type;
			const int level;
		public:
			// Constructs a new item with the given type and level.
			//
			// @param type the type of the item.
			// @param level the level of the item.
			// @throw ItemInvalidArgsException if item type is not potion
			// or candy or if level is negative.
			Item(const ItemType& type, const int& level);

			// Returns the type of the item - candy or potion.
			//
			// @return the type of the item - candy or potion.
			ItemType getItemType() const;

			// Returns the level of the item.
			//
			// @return the level of the item.
			int getItemLevel() const;

			bool operator==(const Item& rhs) const;
		};
	}
}




#endif //POKEMON_ITEM_H
