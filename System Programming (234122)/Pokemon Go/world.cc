//
// Created by Roy on 23/01/2017.
//

//
// Created by Roy on 22/01/2017.
//
#include "world.h"

static const std::string CHECK_STRUCTURE="check_thes_structure_of_the_input";

namespace mtm {
    namespace pokemongo {

//------------helper declarations---------
        ItemType stringToEnum(std::string str);

//------------helper functions---------
        ItemType stringToEnum(std::string str) {
            if (str.compare("POTION")==0)
                return POTION;
            return CANDY;
        }

//return the number if double. return -1 if not int
        double checkIfDouble(std::string str) {
            const char *s = str.c_str();
            double n = std::atof(s);
            return n; //if 0 - bad input
        }

//return the number if int. return -1 if not int
        int checkIfInt(std::string str) {
            const char *s = str.c_str();
            double n = checkIfDouble(s);
            if (n == 0)
                return 0;
            if (n == (int) n)
                return n;
            return 0;
        }

//---------------implementations---------------------
        void World::ifLocationNameExists(std::string location) const {
            if ((*this).Contains(location))
                throw WorldLocationNameAlreadyUsed();
        }


        std::istream &World::createGym(std::istream &input) {
            std::string name;
            name = CHECK_STRUCTURE;
            input >> name;
            if (name.compare(CHECK_STRUCTURE) == 0)
                throw WorldInvalidInputLineException();
            (*this).ifLocationNameExists(name);
            Gym* gym= new Gym;
            Insert(name, gym);
            locationsNamesVec.push_back(name);
            return input;
        }


        std::istream &World::createPokestop(std::istream &input) {
            std::string name;
            input >> name;
            (*this).ifLocationNameExists(name);
            std::string type, level;
            int level_good;
            Pokestop* pokestop=new Pokestop;
            while (input >> type >> level) {
                if (type.compare("POTION") != 0 && type.compare("CANDY") != 0)
                    throw WorldInvalidInputLineException();
                level_good = checkIfInt(level);
                if (level_good == 0)
                    throw WorldInvalidInputLineException();
                try {
                    Item item(stringToEnum(type), level_good);
                    pokestop->pokestopPush(item);
                }
                catch (ItemInvalidArgsException) {
                    throw WorldInvalidInputLineException();
                }
                type = CHECK_STRUCTURE;
                level = type;
            }
            if (!(type.compare(CHECK_STRUCTURE) == 0 &&
                  level.compare(CHECK_STRUCTURE) == 0))
                throw WorldInvalidInputLineException();
            this->Insert(name, pokestop);
            locationsNamesVec.push_back(name);
            return input;
        }


        std::istream &World::createStarbucks(std::istream &input) {
            std::string name;
            input >> name;
            (*this).ifLocationNameExists(name);
            std::string species, cp, level;
            double cp_good;
            int level_good;
            Starbucks* starbucks= new Starbucks;
            while (input >> species >> cp >> level) {
                cp_good = checkIfDouble(cp);
                level_good = checkIfInt(level);
                if (cp_good == 0 || level_good == 0)
                    throw WorldInvalidInputLineException();
                try {
                    Pokemon pokemon(species, cp_good, level_good);
                    starbucks->pushToBranch(pokemon);
                }
                catch (PokemonInvalidArgsException) {
                    throw WorldInvalidInputLineException();
                }
                species = CHECK_STRUCTURE;
                cp = species;
                level = cp;
            }
            if (!(cp.compare(CHECK_STRUCTURE) == 0 &&
                  level.compare(CHECK_STRUCTURE) == 0 &&
                  species.compare(CHECK_STRUCTURE) == 0))
                throw WorldInvalidInputLineException();
            this->Insert(name, starbucks);
            locationsNamesVec.push_back(name);
            return input;
        }


        std::istream &operator>>(std::istream &input, World &world) {
            std::string location;
            input >> location;
            if (location.compare("POKESTOP") == 0) {
                return world.createPokestop(input);
            } else if (location.compare("STARBUCKS") == 0) {
                return world.createStarbucks(input);
            } else if (location.compare("GYM") == 0) {
                return world.createGym(input);
            } else
                throw WorldInvalidInputLineException();
        }
    }
}