#include "pokemon.h"
#include "exceptions.h"

const int MAX_HP=100;

namespace mtm {
	namespace pokemongo {

//------------helper declarations---------
int setToSum(std::set<PokemonType> typesSet);
// Returns which pokemon is stronger (or tied).
//
// @return 0 if pokemons are tied.
// 		   1 if this pokemom is stronger.
// 		   2 if given pokemon (as parameter) is stronger.

int isBigger (std::set<PokemonType> typesSet1, std::set<PokemonType> typesSet2);

// Returns a string equivalent to the pokemon type (enum)
// @param the pokemon type enum
// @return pokemon type as string
std::string enumToPokemonType (const int pokemonEnum);

// Returns a string with all of the pokemons types combined
// @param a set of pokemon types (enums)
// @return string with pokemon types
std::string typesToCombinedString (std::set<PokemonType> typesSet);
//------------helper functions---------

int setToSum(std::set<PokemonType> typesSet){
	int sum=0;
	for(int num : typesSet)
		sum+=num;
	return sum;
}

int isBigger (std::set<PokemonType> typesSet1, std::set<PokemonType> typesSet2){
	int sum1=0, sum2=0;
	sum1=setToSum(typesSet1);
	sum2=setToSum(typesSet2);
	if(sum1>sum2)
		return 1;
	if(sum2>sum1)
		return 2;
	return 0;
}


std::string enumToPokemonType (const int pokemonEnum){
		switch(pokemonEnum){
			case 0:
				return "NORMAL ";
			case 1:
				return "ROCK ";
			case 2:
				return "BUG ";
			case 3:
				return "FAIRY ";
			case 4:
				return "GROUND ";
			case 5:
				return "GRASS ";
			case 6:
				return "WATER ";
			case 7:
				return "ICE ";
			case 8:
				return "GHOST ";
			case 9:
				return "POISON ";
			case 10:
				return "ELECTRIC ";
			case 11:
				return "FIRE ";
			case 12:
				return "FLYING ";
			case 13:
				return "PSYSHIC ";
		}
			return "";
}


std::string typesToCombinedString (std::set<PokemonType> typesSet){
	std::string str=" ";
	for(int num : typesSet)
		str+=enumToPokemonType(num);
	str.pop_back();
	return str;
}

//------------header implementation functions---------
Pokemon::Pokemon(const std::string &species,
				 const std::set<PokemonType> &types,
				 const double &cp,
				 const int &level) :
	level(level), hp(MAX_HP), cp(cp), species(species), types(types) {
	PokemonInvalidArgsException err;
	if (level <= 0 || cp <= 0 || species.length() == 0)
	throw err;
}

Pokemon::Pokemon(const std::string &species,
				 const double &cp, const int &level) :
	level(level), hp(MAX_HP), cp(cp), species(species),
	types(GetDefaultTypes(species)) {
		PokemonInvalidArgsException err;
		if (level <= 0 || cp <= 0 || species.length() == 0)
			throw err;
	}


int Pokemon::Level() const {
	return level;
}

void Pokemon::Heal() {
	hp = MAX_HP;
}


void Pokemon::Train(const double& boost){
	if (boost <= 1) {
		PokemonInvalidArgsException err;
		throw err;
	}
	cp*=boost;
}


bool Pokemon::Hit(Pokemon &victim) {
	double hitPower=cp*level;
	victim.hp -= hitPower;
	if (victim.hp <= 0) {
		victim.hp = 0;
		return true;
	}
	return false;
}

bool Pokemon::operator>(const Pokemon& rhs) const{
	if(cp*level>rhs.cp*rhs.level)
		return true;
	if(cp*level==rhs.cp*rhs.level)
		return isBigger(types,rhs.types) == 1;
	return false;
}

bool Pokemon::operator<(const Pokemon& rhs) const{
	return rhs>*this;
}

bool Pokemon::operator<=(const Pokemon& rhs) const{
	return !(*this>rhs);
}

bool Pokemon::operator>=(const Pokemon& rhs) const{
	return !(*this<rhs);
}

bool Pokemon::operator!=(const Pokemon& rhs) const{
	return (*this>rhs) || (*this<rhs);
}

bool Pokemon::operator==(const Pokemon& rhs) const{
	if(!(*this>rhs || *this<rhs)) //if tied
		return true;
	return false;
}





std::ostream& operator<<(std::ostream& output,
						 const Pokemon& pokemon){
	return output << pokemon.species << "(" << pokemon.level << "/" <<
		   pokemon.cp << "/" << pokemon.hp << ")" <<
			typesToCombinedString(pokemon.types);
}

} //namespace pokemongo
} //namespace mtm