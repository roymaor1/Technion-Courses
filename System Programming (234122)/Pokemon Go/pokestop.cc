//
// Created by Roy on 22/01/2017.
//

#include "pokestop.h"
#include "item.h"
#include "location.h"

namespace mtm {
	namespace pokemongo {

		Pokestop::Pokestop() :
				items(std::list<Item>()) {
		}

		std::list<Item> Pokestop::getItemsList(){
			return items;
		}
		void Pokestop::Arrive(Trainer& trainer) {
			if (std::find(trainers_.begin(), trainers_.end(), &trainer) !=
				trainers_.end()) {
				throw LocationTrainerAlreadyInLocationException();
			}
			trainers_.push_back(&trainer);
			grabAvailableItem();
		}
		bool Pokestop::grabAvailableItem (){
			if(items.empty())
				return false;
			int trainer_level = (*(trainers_.back())).getLevel();
			std::list<Item>::iterator i = items.begin();
			for(; i!=items.end(); ++i) {
				if (trainer_level >= (*i).getItemLevel()) {
					(*(trainers_.back())).pushItem(*i); //pushes the pointer
					items.erase(i); //erases the pointer/iterator
					return true;
				}
			}
				return false;
		}

		void Pokestop::pokestopPush(Item& item){
			items.push_back(item); //push to end
		}

		Item& Pokestop::pokestopTop(){
			return items.front();
		}

		void Pokestop::pokestopPop(){
			items.pop_front();
		}

		void Pokestop::pokestopPrint() {
				std::list<Item>::const_iterator i;
				for( i = items.begin(); i != items.end(); ++i)
					std::cout << (*i).getItemLevel() << std::endl;
			}
		}

	}
