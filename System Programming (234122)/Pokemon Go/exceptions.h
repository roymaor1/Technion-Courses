#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

namespace mtm {

	class MtmException {};

	class KGraphExcpetion : public MtmException {};
	class KGraphKeyNotFoundException : public KGraphExcpetion {};
	class KGraphKeyAlreadyExistsExpection : public KGraphExcpetion {};
	class KGraphIteratorReachedEnd : public KGraphExcpetion {};
	class KGraphEdgeOutOfRange : public KGraphExcpetion {};
  class KGraphEdgeAlreadyInUse : public KGraphExcpetion {};
  class KGraphNodesAlreadyConnected : public KGraphExcpetion {};
  class kGraphNodesAreNotConnected : public KGraphExcpetion {};

namespace pokemongo {

	class PokemonException : public MtmException {};
	class PokemonInvalidArgsException : public PokemonException {};

	class TrainerException : public MtmException {};
	class TrainerNoPokemonsFoundException : public TrainerException {};
  class TrainerInvalidArgsException : public TrainerException {};

	class LocationException : public MtmException {};
  class LocationTrainerNotFoundException : public LocationException {};
  class LocationTrainerAlreadyInLocationException : public LocationException {};

	class GymException : public LocationException {};
	class PokemonGoGymHasNoLeader : public GymException{};

	class ItemException : public MtmException {};
	class ItemInvalidArgsException : public ItemException {};

	class WorldException : public MtmException {};
  class WorldInvalidInputLineException : public WorldException {};
	class WorldLocationNameAlreadyUsed : public WorldException {};

	class PokemonGoException : public MtmException {};
	class PokemonGoLocationNotFoundException : public PokemonGoException {};
	class PokemonGoTrainerNameAlreadyUsedExcpetion : public PokemonGoException {};
  class PokemonGoInvalidArgsException : public PokemonGoException {};
  class PokemonGoReachedDeadEndException : public PokemonGoException {};
  class PokemonGoTrainerNotFoundExcpetion : public PokemonGoException {};
	class PokemonGoTrainerNeutralType : public PokemonGoException{};


}  //  namespace pokemongo
}  //  namespace mtm

#endif  // EXCEPTIONS_H