#ifndef WORLD_H
#define WORLD_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <string>
#include <cstring>
#include "item.h"
#include "k_graph.h"
#include "location.h"
#include "pokestop.h"
#include "starbucks.h"
#include "gym.h"
#include <vector>
#include "exceptions.h"

const int NUM_DIRECTIONS=4;

namespace mtm {
namespace pokemongo {

typedef int Direction;

static const int NORTH = 0;
static const int SOUTH = 1;
static const int EAST = 2;
static const int WEST = 3;

class World : public KGraph<std::string, Location*, NUM_DIRECTIONS>{
 public:
  // Constructs a new empty world.
	std::vector<std::string> locationsNamesVec;
  World() : KGraph<std::string, Location*, NUM_DIRECTIONS>(nullptr),
			locationsNamesVec(std::vector<std::string>()){}
  
  // A destructor.
  ~World(){    for(auto it=locationsNamesVec.begin();
				   it!=locationsNamesVec.end();++it){
		  Location* locationToDelete=this->operator[](*it);
		  delete locationToDelete;
	  }

  }
  
  // Input iterator. Scans a single line from the input stream. The line can be
  // one of the following three options:
  //
  // (1) "GYM <gym_name>"
  //     e.g. "GYM taub"
  //     Adds a gym with the given name to the world.
  // (2) "POKESTOP <pokestop_name> <item_type1> <item_level1> <item_type2> <item_level2> ..."
  //     e.g. "POKESTOP mikhlol POTION 10 CANDY 20 CANDY 13"
  //     Creates a Pokestop with the given name that contains the given items by
  //     order, and adds it to the world.
  // (3) "STARBUCKS <starbucks_name> <pokemon_species1> <cp1> <level1> <pokemon_species2> <cp2> <level2> ..."
  //     e.g. "STARBUCKS shani pikachu 2.5 3 pikachu 1 2 charmander 3.45 4"
  //     Creates a starbucks with the given name that contains Pokemons with the
  //     given parameters by order, and adds it to the world. The Pokemons will
  //     have the default types.
  // You can assume that none of the pieces of information (location name,
  // Pokemon species, etc.) contains a space.
  //
  // @param input the input stream.
  // @param world the world to which to add the locations.
  // @return the input stream.
  // @throw WorldInvalidInputLineException if the input line is not one of the
  //        three options, or one of the parameters is invalid (for example,
  //        negative CP value, etc.).
  // @throw WorldLocationNameAlreadyUsed if there already exists a location with
  //        the given name in the world.
  friend std::istream& operator>>(std::istream& input, World& world);

  // Disable copy constructor.
  World(const World& world) = delete;

///
/// \param input
/// \return
	std::istream& createGym(std::istream& input)  ;
///
/// \param input
/// \return
	std::istream& createPokestop(std::istream& input) ;

///
/// \param input
/// \return
	std::istream& createStarbucks(std::istream& input) ;

///
/// \param location
	void ifLocationNameExists (std::string location) const;
	};

std::istream& operator>>(std::istream& input, World& world);

}  // namespace pokemongo
}  // namespace mtm

#endif  // WORLD_H