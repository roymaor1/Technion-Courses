//
// Created by Roy on 22/01/2017.
//

#ifndef POKEMON_POKESTOP_H
#define POKEMON_POKESTOP_H

#include <list>
#include "location.h"
#include "trainer.h"
#include "exceptions.h"
#include "item.h"


namespace mtm {
	namespace pokemongo {
		class Pokestop : public Location{
			std::list<Item> items;

		public:
			// Constructs a new pokestop.
			//
			Pokestop();

			// pop first item in pok
			// @return vector of items of the trainer.
			void pokestopPop(); //FOR TESTS

			// returns a reference to the first item in list.
			// @return a reference to the first item in list.
			Item& pokestopTop(); //FOR TESTS

			// add an item to the end of the list.
			//
			void pokestopPush(Item& item); //FOR TESTS

			// Returns vector of items of the trainer.
			// @return vector of items of the trainer.
			std::list<Item> getItemsList();


			// Lets trainer grab an item from the pokestop, if possible.
			// @param trainer the trainer trying to grab item
			// @return TRUE if trainer grabbed an item, FALSE if he hasnt.
			bool grabAvailableItem ();

			void pokestopPrint();

			virtual void Arrive(Trainer& trainer) override;

		};
	}
}
#endif //POKEMON_POKESTOP_H
