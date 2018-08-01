#ifndef POKEMON_H
#define POKEMON_H

#include <iostream>
#include <set>
#include <string>

namespace mtm {
namespace pokemongo {

// Possible Pokemon types.
typedef enum {
  NORMAL = 0,
  ROCK = 1,
  BUG = 2,
  FAIRY = 3,
  GROUND = 4,
  GRASS = 5,
  WATER = 6,
  ICE = 7,
  GHOST = 8,
  POISON = 9,
  ELECTRIC = 10,
  FIRE = 11,
  FLYING = 12,
  PSYCHIC = 13
} PokemonType;

class Pokemon {
	int level; //private. should not be const (bc it is assigned later)
	double hp, cp; //private
	std::string species; //private. TODO: const?
    std::set<PokemonType> types; //private
public:
	std::string getPokemonName(){
		return species;
	}
  // Computes the default set of Pokemon types for a given species of Pokemons.
  //
  // @param species the species of the Pokemon.
  // @return the default set of types for the given species.
  static std::set<PokemonType> GetDefaultTypes(const std::string& species);

  // Constructs a new Pokemon with the specified data.
  //
  // @param species the species of the Pokemon.
  // @param types the types of the Pokemon.
  // @param cp the CP value of the Pokemon.
  // @param level the level of the Pokemon.
  // @throw PokemonInvalidArgsException if a non-positive level or CP value were
  //        passed, or if species is an empty string.
  Pokemon(const std::string& species,
          const std::set<PokemonType>& types,
          const double& cp,
          const int& level);
  
  // Constructs a new Pokemon with the specified data and the default types for
  // its species.
  //
  // @param species the species of the Pokemon.
  // @param cp the CP value of the Pokemon.
  // @param level the level of the Pokemon.
  // @throw PokemonInvalidArgsException if a non-positive level or CP value were
  //        passed, or if species is an empty string.
  Pokemon(const std::string& species, const double& cp, const int& level);
  
  // Copy constructor.
  //
  // @param pokemon the pokemon to copy.
  Pokemon(const Pokemon& pokemon) = default;
  
  // Assignment operator.
  //
  // @param pokemon assignee.
  Pokemon& operator=(const Pokemon& pokemon) = default;

  // Comparison operators for Pokemons. Pokemons are compared as described in
  // the exercise sheet.
  //
  // @param rhs the right-hand side operand.
  bool operator==(const Pokemon& rhs) const;
  bool operator!=(const Pokemon& rhs) const;
  bool operator<(const Pokemon& rhs) const;
  bool operator>(const Pokemon& rhs) const;
  bool operator<=(const Pokemon& rhs) const;
  bool operator>=(const Pokemon& rhs) const;
  
  // Prints the data of the Pokemon in the following format:
  //
  //     "<species> (<level>/<CP>/<HP>) <type1> <type2> ..."
  //
  // where the types are ordered by their numerical value.
  // e.g.
  //
  //     pikachu (3/5.5/87) NORMAL FIRE FLYING
  //
  // @param output the output stream to which the data is printed.
  // @param pokemon the pokemon whose data is printed.
  // @return the output stream to which the data is printed.
  friend std::ostream& operator<<(std::ostream& output, const Pokemon& pokemon);

  // Returns the level of the Pokemon.
  //
  // @return the level of the Pokemon.
  int Level() const;

  // "Hits" the given Pokemon by reducing its HP value by the hit power of this
  // Pokemon.
  //
  // @param the Pokemon to hit.
  // @return true iff the hitten Pokemon died (HP reached zero).
  bool Hit(Pokemon& victim);

  // Heals the Pokemon by increasing its HP to 100.
  void Heal();

  // Trains the Pokemon by multiplying its CP value by the given boost.
  //
  // @param boost the boost by which the CP value is multiplied.
  // @throw PokemonInvalidArgsException if boost is not bigger than 1.
  void Train(const double& boost);
};

std::ostream& operator<<(std::ostream& output, const Pokemon& pokemon);


}  // namespace pokemongo
}  // namespace mtm

#endif  // POKEMON_H
