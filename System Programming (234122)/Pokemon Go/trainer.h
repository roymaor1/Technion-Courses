#ifndef TRAINER_H
#define TRAINER_H


#include <iostream>
#include <string>
#include <list>
#include <queue>
#include "pokemon.h"
#include "item.h"
#include "exceptions.h"




namespace mtm {
    namespace pokemongo {


// Teams in game.
        typedef enum {
            BLUE,
            YELLOW,
            RED,
            NEUTRAL
        } Team;

        class Trainer {
            std::string name;
            int level;
            std::list<Pokemon> pokemons;
            Team team;
            std::queue<Item> items;
            std::string trainersLocationName;

            template<typename pokemonOutput, typename pokemonsInput,
                    typename Iterator>
            static pokemonOutput getStrongestPokemonGeneric(pokemonsInput
                                                            pokemons) {
                if (pokemons.empty()) {
                    TrainerNoPokemonsFoundException err;
                    throw err;
                }
                Iterator maxPokemon = pokemons.begin();
                for (Iterator i = pokemons.begin(); i != pokemons.end(); ++i) {
                    if (*i > *maxPokemon)
                        maxPokemon = i;
                }
                return *maxPokemon;
            }

            ///uses the oldest Item on the strongest pokemon
            void useItem();


            ///
            /// \param newLocatin
            void setTrainersLocation(std::string trainersLocationName);
            ///
            /// \param newLocatin
            std::string& getTrainersLocation();
            // Sets the pokemon list of the trainer
            void setPokemonList(const std::list<Pokemon> &list) {
                pokemons = list;
            }

            // Gets the pokemon list of the trainer
            //
            // @return the pokemon list of the trainer.
            std::list<Pokemon> getPokemonList();

            std::queue<Item> getItemList();

            void popItem();

            // Returns the team to which the trainer belongs.
            //
            // @return the team to which the trainer belongs.

            // set level of trainer. ~FOR TESTS ONLY~
            void setLevel(int level);

            // Lets trainer obtain item.
            //
            void pushItem(Item item);



            // Lets trainer obtain pokemon.
            //
            void pushPokemon(Pokemon pokemon);

            // Returns a reference to the "oldest" pokemon of the trainer.
            //
            // @return a reference to the "oldest" pokemon of the trainer.
            Pokemon &topPokemon();

            void popPokemon();


            Item &topItem();


        public:
            //TODO: following function until constructor were added at part 3.
            int *teamScore;

            ///
            /// \return trainer's level
            int getLevel() const;

            // Kills the strongest Pokemon. Removes the Pokemon that is returned
            // from GetStrongestPokemon() from the collection of Pokemons
            // owned by the trainer.
            //
            // @throw TrainerNoPokemonsException if trainer has no Pokemons.

            void KillStrongestPokemon();

            std::string getTrainersName(){
                return name;
            }

            // Constructs a new trainer with the given name and team.
            //
            // @param name the name of the trainer.
            // @param team the team to which the trainer belongs.
            // @throw TrainerInvalidArgsException if name is an empty string.
            Trainer(const std::string &name, const Team &team);

            // Returns a reference to the strongest Pokemon the trainer owns.
            // Strongest Pokemon is determined using the comparison operators
            // provided by the class Pokemon. If two Pokemons are of equal
            // strength, the function returns the
            // one that was caught earlier by the trainer.
            //
            // @return the strongest pokemon.
            // @throw TrainerNoPokemonsException if trainer has no Pokemons.
            Pokemon &GetStrongestPokemon();

            const Pokemon &GetStrongestPokemon() const;


            // Comparison operators for trainers. Trainers are compared
            // as described in the exercise sheet.
            //
            // @param rhs the right-hand side operand.
            bool operator==(const Trainer &rhs) const;

            bool operator!=(const Trainer &rhs) const;

            bool operator<(const Trainer &rhs) const;

            bool operator>(const Trainer &rhs) const;

            bool operator<=(const Trainer &rhs) const;

            bool operator>=(const Trainer &rhs) const;

            // Checks whether a given trainer is in the same team as
            // this trainer.
            //
            // @param trainer
            // @return true iff trainer (argument) and this trainer belong to
            // the same team.
            bool IsAlly(const Trainer &trainer) const;

            // Returns the team to which the trainer belongs.
            //
            // @return the team to which the trainer belongs.
            Team GetTeam() const;

            // Tries to catch a Pokemon.
            //
            // @param pokemon the Pokemon the trainer wishes to catch.
            // @return true if the attempt succeeded.
            bool TryToCatch(Pokemon &pokemon);

            // Prints the data of the trainer in the following format:
            //
            //     "<name> (<level>) <team>\n
            //      <pokemon1>\n
            //      <pokemon2>\n
            //      ..."
            //
            // where the Pokemons are ordered from least recently caught to
            // most recently.
            // e.g.
            //
            //     ash (4) YELLOW
            //     pikachu (3/5.5/87) NORMAL FIRE FLYING
            //     charmander (2/2.5/100) NORMAL BUG
            //     pikachu (7/6.5/56) FIRE
            //
            //
            // @param output the output stream to which the data is printed.
            // @param pokemon the pokemon whose data is printed.
            // @return the output stream to which the data is printed.
            friend std::ostream &operator<<(std::ostream &output, const
            Trainer &trainer);

            /// prefomers level update as if trainer1 wins
            /// \param trainer1 - winner
            /// \param trainer2- loser
            friend void inline
            firtsTrainerWins(Trainer &trainer1, Trainer &trainer2);

            /// returns winner after changing level
            /// \param trainer1
            /// \param trainer2
            /// \param winner -wiiner number
            /// \return
            friend Trainer &returnWinner(Trainer &trainer1, Trainer &trainer2,
                                         int winner);
            /// preforms battle fight between 2 trainers
            /// \@param
            /// \@return a refference to the winning trainer
            friend Trainer &TrainersFight(Trainer &trainer1, Trainer &trainer2);

            /// returns the winning by trainers color team
            /// \param trainer1 - first trainer
            /// \param trainer2 - second trainer
            /// \return
            friend Trainer &winningTrainerByTeam(Trainer &trainer1,
                                                 Trainer &trainer2);
            friend class Location;
            friend class PokemonGo;
            friend class Pokestop;
            friend class Starbucks;
            friend class Gym;
            ///
            /// \param trainer1
            /// \param trainer2
            /// \param levelCh
            friend void inline
            firtsTrainerWins(Trainer &trainer1,Trainer &trainer2,bool levelCh);
        };

std::ostream& operator<<(std::ostream& output, const Trainer& trainer);

}  // namespace mtm
}  // namespace pokemongo

#endif  // TRAINER_H