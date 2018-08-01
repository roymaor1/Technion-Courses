//
// Created by Roy on 22/01/2017.
//

#include "starbucks.h"

namespace mtm {
	namespace pokemongo {

		Starbucks::Starbucks() :
				pokemons(std::queue<Pokemon>()) {
		}

		Pokemon& Starbucks::starbucksTop(){
			return pokemons.front();
		}

		void Starbucks::pushToBranch(Pokemon& pokemon){
			pokemons.push(pokemon);
		}

		void Starbucks::popFromBranch(){
			pokemons.pop();
		}

		bool Starbucks::grabAvailablePokemon (){
			if(pokemons.empty())
				return false;
			int pokemon_level = (pokemons.front()).Level();
			int trainer_level = (*(trainers_.back())).getLevel();
			if(trainer_level>=pokemon_level){
				(*(trainers_.back())).pushPokemon((pokemons.front())); //assign to trainer
				pokemons.pop(); //remove from starbucks
				return true;
			}
			return false;
		}

		void Starbucks::Arrive(Trainer& trainer){
			if (std::find(trainers_.begin(), trainers_.end(), &trainer) !=
				trainers_.end()) {
				throw LocationTrainerAlreadyInLocationException();
			}
			trainers_.push_back(&trainer);
			grabAvailablePokemon();

		}

	}
}