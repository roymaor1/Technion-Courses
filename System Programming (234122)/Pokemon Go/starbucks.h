//
// Created by Roy on 22/01/2017.
//

#ifndef POKEMON_STARBUCKS_H
#define POKEMON_STARBUCKS_H


#include <queue>

#include "exceptions.h"
#include "item.h"
#include "location.h"

namespace mtm {
	namespace pokemongo {

		class Starbucks : public Location{
			std::queue<Pokemon> pokemons;
		public:
			// Constructs a new starbucks branch.
			//
			Starbucks();

			// Pushes a pokemon to this starbucks branch.
			//
			void pushToBranch(Pokemon& pokemon);

			// Pushes a pokemon to this starbucks branch.
			//
			void popFromBranch();

			// Returns reference to the "oldest" pokemon in queue.
			// @return vector of items of the trainer.
			Pokemon& starbucksTop();

			// Lets trainer grab a pokemon from starbucks, if possible.
			// @param trainer the trainer trying to grab pokemon
			// @return TRUE if trainer grabbed a pokemon, FALSE if he hasnt.
			bool grabAvailablePokemon();

			virtual void Arrive(Trainer& trainer) override;

		};
	}
}


#endif //POKEMON_STARBUCKS_H
