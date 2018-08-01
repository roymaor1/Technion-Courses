//
// Created by Roy on 19/01/2017.
//
#include "trainer.h"
#include "pokemon.h"
#include "exceptions.h"
#include <sstream>
#include <cmath>



const int DEFAULT_LEVEL=1;



namespace mtm {
	namespace pokemongo {

//------------helper declarations---------
		std::string enumToTeam (const int teamEnum);

		std::string printPokemonList (const std::list<Pokemon> list);

//------------helper functions---------
		std::string enumToTeam (const int teamEnum){
			switch(teamEnum){
				case 0:
					return "BLUE";
				case 1:
					return "YELLOW";
				case 2:
					return "RED";
			}
			return "";
		}

		std::string printPokemonList (const std::list<Pokemon> list){
			std::string str="";
			if(list.empty())
				return str;
			std::ostringstream stream;
			for(std::list<Pokemon>::const_iterator i = list.begin(); i!=list.end(); ++i){
				stream << *i << std::endl;
			}
			str=stream.str();
			return str;
		}
		//------------header implementation functions---------
		Trainer::Trainer(const std::string &name, const Team &team):name(name),
				level(DEFAULT_LEVEL),pokemons(std::list<Pokemon>()),team(team),
				items(std::queue<Item>()){
			TrainerInvalidArgsException err;
			if (name.length() == 0)
				throw err;
		}

		Pokemon& Trainer::GetStrongestPokemon(){
			return getStrongestPokemonGeneric<Pokemon&,std::list<Pokemon>&,
					std::list<Pokemon>::iterator>(pokemons);
		}

		const Pokemon& Trainer::GetStrongestPokemon() const{
			return getStrongestPokemonGeneric<const Pokemon&,const std::list<Pokemon>&,
					std::list<Pokemon>::const_iterator>(pokemons);
		}


		void Trainer::KillStrongestPokemon(){
			if(pokemons.empty()){
				TrainerNoPokemonsFoundException err;
				throw err;
			}
			pokemons.remove(GetStrongestPokemon());
		}

		bool Trainer::operator>(const Trainer& rhs) const{
			if(pokemons.empty())
				return false;
			if(rhs.pokemons.empty())
				return true;
			return this->GetStrongestPokemon()>rhs.GetStrongestPokemon();
		}

		bool Trainer::operator<(const Trainer& rhs) const{
			return rhs>*this;
		}

		bool Trainer::operator<=(const Trainer& rhs) const{
			return !(*this>rhs);
		}

		bool Trainer::operator>=(const Trainer& rhs) const{
			return !(*this<rhs);
		}

		bool Trainer::operator==(const Trainer& rhs) const{
			return !(*this>rhs) && !(*this<rhs);
		}

		bool Trainer::operator!=(const Trainer& rhs) const{
			return !(*this==rhs);
		}

		Team Trainer::GetTeam() const{
			return team;
		}

		bool Trainer::IsAlly(const Trainer& trainer) const{
			return team==trainer.GetTeam();
		}

		bool Trainer::TryToCatch(Pokemon& pokemon){
			if(level>=pokemon.Level()){
				pokemons.push_back(pokemon);
				return true;
			}
			return false;
		}

		int Trainer::getLevel() const{
			return level;
		}

		std::list<Pokemon> Trainer::getPokemonList(){
			return pokemons;
		}

		void Trainer::pushItem(Item item){
			items.push(item);
		}

		void Trainer::pushPokemon(Pokemon pokemon) {
			pokemons.push_back(pokemon); //push to end
		}

		void Trainer::setLevel(int level){
			this->level=level;
		}

		Pokemon& Trainer::topPokemon(){
			return pokemons.front();
		}

		void Trainer::popPokemon(){
			pokemons.pop_front();
		}

		Item& Trainer::topItem(){
			return items.front();
		}

		void Trainer::popItem(){
			items.pop();
		}


		std::queue<Item> Trainer::getItemList(){
			return items;
		}


		std::ostream& operator<<(std::ostream& output, const Trainer& trainer){
			return output << trainer.name << " (" << trainer.level << ") " <<
						  enumToTeam(trainer.team) << std::endl <<
						  printPokemonList(trainer.pokemons);
		}



void inline firtsTrainerWins(Trainer &trainer1,Trainer &trainer2,bool levelCh){
			(*trainer1.teamScore)+=2;
			(*trainer2.teamScore)-=1;
	if(levelCh){
		int delta= ceil(double(trainer2.getLevel())/2);
		trainer1.setLevel(trainer1.getLevel()+delta);
		(*trainer1.teamScore)+=delta;
	}

}
Trainer& returnWinner(Trainer& trainer1,Trainer& trainer2, int winner){
	switch (winner){
		case 1:
			firtsTrainerWins(trainer1, trainer2, true);
			return trainer1;
		case 2:
			firtsTrainerWins(trainer2, trainer1, true);
			return trainer2;
		default:
			return winningTrainerByTeam(trainer1,trainer2);
	}
}

Trainer& TrainersFight(Trainer& trainer1,Trainer& trainer2){
	if(trainer1.pokemons.empty()){
		if(!trainer2.pokemons.empty()) {
			firtsTrainerWins(trainer2, trainer1, true);
			return trainer2;
		}
		return winningTrainerByTeam(trainer1,trainer2);
	} else if(trainer2.pokemons.empty()){
		firtsTrainerWins(trainer1, trainer2, true);
		return trainer1;
	}
	trainer1.useItem();
	trainer2.useItem();
	int winner=0;
	if(trainer1>trainer2)
		winner=1;
	if(trainer2>trainer1)
		winner=2;
	bool isSecondDied=trainer1.GetStrongestPokemon().Hit(trainer2.
			GetStrongestPokemon());
	if (trainer2.GetStrongestPokemon().Hit(trainer1.GetStrongestPokemon()))
		trainer1.KillStrongestPokemon();
	if(isSecondDied)
		trainer2.KillStrongestPokemon();
	return returnWinner(trainer1,trainer2,winner);
}



void Trainer::useItem() {
	if(items.empty())
		return;
	Item itemToUse=items.front();
	if(itemToUse.getItemType()==POTION){
		GetStrongestPokemon().Heal();
	} else{
		Pokemon& trainedPokemon = GetStrongestPokemon();
		trainedPokemon.Train((double(trainedPokemon.Level())/10)+1);
	}
	items.pop();
	return;
}

Trainer& winningTrainerByTeam(Trainer& trainer1, Trainer& trainer2){
	Team team1=trainer1.GetTeam();
	Team team2=trainer2.GetTeam();
	int winner;
	switch (team1) {
		case BLUE:
			if (team2 == RED)
				winner=1;
			if(team2==YELLOW)
				winner=2;
			break;
		case YELLOW:
			if(team2==BLUE)
				winner=1;
			if(team2==RED)
				winner=2;
			break;
		case RED:
			if(team2==YELLOW)
				winner=1;
			if(team2==BLUE)
				winner=2;
			break;
		default: 	throw PokemonGoTrainerNeutralType();
	}
	if(winner==1){
		firtsTrainerWins(trainer1, trainer2, false);
		return trainer1;
	}

	if(winner==2){
		firtsTrainerWins(trainer2, trainer1, false);
		return trainer2;
	}
	//here only when trainer are from the same team
	return trainer1>trainer2 ? trainer1 : trainer2;
}
void Trainer::setTrainersLocation(std::string trainersLocationName){
	if(trainersLocationName=="")
		throw PokemonGoLocationNotFoundException();
	this->trainersLocationName=trainersLocationName;
	return;

}
std::string& Trainer::getTrainersLocation(){
	return trainersLocationName;
}

	}
}