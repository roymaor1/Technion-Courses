#ifndef POKEMON_GO_H
#define POKEMON_GO_H

#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <string>
#include "world.h"
#include "trainer.h"
#include <iostream>

namespace mtm {
namespace pokemongo {

class PokemonGo {
    const World* gameWorld;
    std::map<std::string,Trainer> trainersMap;
    int yellowScore,blueScore,redScore;
 public:
  // Initilaizes a new game with the given world. This passes ownership of
  // world, meaning the constructed PokemonGo is responsible for deleting all
  // memory allocated by world.
  //
  // @param world the world in which the game takes place.
  PokemonGo(const World* world);
  
  // Disable copy and assignment.
  PokemonGo(const PokemonGo&) = delete;

  // A destructor.
  ~PokemonGo();
  
  // Adds a new trainer to the game to the specified location.
  //
  // @param name the name of the new trainer.
  // @param team the team to which the new trainer belongs.
  // @param location the name of the location where the new trainer starts the
  //        game.
  // @throw PokemonGoInvalidArgsException if the name of the trainer is an empty
  //        string.
  // @throw PokemonGoTrainerNameAlreadyUsedExcpetion if there already exists a
  //        trainer with the given name in the game.
  // @throw PokemonGoLocationNotFoundException if the specified location does
  //        not exist.
  void AddTrainer(
      const std::string& name, const Team& team, const std::string& location);

  // Moves a trainer from one location to another in the specified direction.
  //
  // @param trainer_name the name of the trainer to be moved.
  // @param dir the direction in which the trainer should be moved.
  // @throw PokemonGoTrainerNotFoundExcpetion in there exists no trainer with
  //        the given name in the game.
  // @throw PokemonGoReachedDeadEndException if the direction specified does not
  //        lead to any other location.
  void MoveTrainer(const std::string& trainer_name, const Direction& dir);

  // Returns the name of the location of the given trainer.
  //
  // @param trainer_name the name of the trainer.
  // @return the name of the location of the trainer.
  // @throw PokemonGoTrainerNotFoundExcpetion in there exists no trainer with
  //        the given name in the game.
  std::string WhereIs(const std::string& trainer_name);

  // Returns a vector of the trainers that are found in the specified location.
  // The order of the trainer is by their last arrival time to the location:
  // from earliest to latest.
  //
  // @param location the name of the location.
  // @return the set of trainers found in the given location.
  // @throw PokemonGoLocationNotFoundException if the specified location does
  //        not exist.
  const std::vector<Trainer*>& GetTrainersIn(const std::string& location);

  // Returns the score of a given team in the game.
  //
  // @param team
  // @return the score of team.
  int GetScore(const Team& team);
};

}  // namespace pokemongo
}  // namespace mtm

#endif // POKEMON_GO_H