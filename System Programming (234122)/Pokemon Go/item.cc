//
// Created by Roy on 22/01/2017.
//

#include "item.h"
const int NUM_TYPES=2;

namespace mtm {
	namespace pokemongo {

		Item::Item(const ItemType& type, const int& level) :
				type(type), level(level) {
			ItemInvalidArgsException err;
			if (type<0 || type>NUM_TYPES-1 || level<1)
				throw err;
		}

		ItemType Item::getItemType() const{
			return type;
		}

		int Item::getItemLevel() const{
			return level;
		}

		bool Item::operator==(const Item& rhs) const{
			return type==rhs.type && level==rhs.level;
		}

	}
}