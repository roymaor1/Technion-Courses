#ifdef P
#include "pokemon.h"

#include <iostream>

#include "exceptions.h"
#include "test_utils.h"

using namespace std;
using namespace mtm;
using namespace mtm::pokemongo;

namespace {
bool VerifyOutput(const ostringstream& output, const string& expected_output) {
  ASSERT_EQUAL(expected_output, output.str());
  return true;
}
}

bool TestInvalidArgs() {
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", 1, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", 0, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", -1, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", 1, 0));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", 0, 0));
  
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("", {FIRE}, 1, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", {FIRE}, 0, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", {FIRE}, -1, 1));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", {FIRE}, 1, 0));
  ASSERT_THROW(PokemonInvalidArgsException, Pokemon("pikachu", {FIRE}, 0, 0));
  
  return true;
}

bool TestDefaultTypes() {
  ostringstream output;
  Pokemon pikachu("pikachu", 3, 1);
  Pokemon charmander_empty("charmander", {}, 3, 1);
  Pokemon charmander_def("charmander", 2.5, 7);
  Pokemon signs("?!$%^", 1, 1);

  output << pikachu << endl;
  output << charmander_empty << endl;
  output << charmander_def << endl;
  output << signs << endl;

  return VerifyOutput(output,
                      "pikachu(1/3/100)\n"
                      "charmander(1/3/100)\n"
                      "charmander(7/2.5/100) NORMAL BUG FAIRY ICE GHOST\n"
                      "?!$%^(1/1/100) NORMAL POISON ELECTRIC FIRE\n");
}

bool TestCopyAndAssignment() {
  ostringstream output;
  Pokemon pikachu("pikachu", {FIRE, FLYING}, 3, 1);
  Pokemon charmander("charmander", {POISON, ELECTRIC}, 3, 1);
  Pokemon pikachu_copy(pikachu);
  Pokemon charmander_assigned("temp", {FLYING}, 7, 7);
  charmander_assigned = charmander;

  output << pikachu << endl;
  output << charmander << endl;
  output << pikachu_copy << endl;
  output << charmander_assigned << endl;

  return VerifyOutput(output,
                      "pikachu(1/3/100) FIRE FLYING\n"
                      "charmander(1/3/100) POISON ELECTRIC\n"
                      "pikachu(1/3/100) FIRE FLYING\n"
                      "charmander(1/3/100) POISON ELECTRIC\n");
}

bool TestComparison () {
  Pokemon pokemons[6] = {Pokemon("0", {}, 1, 1),
                         Pokemon("1", {FIRE}, 1, 1),
                         Pokemon("2", {}, 2, 1),
                         Pokemon("3", {FAIRY, GROUND}, 2, 1),
                         Pokemon("4", {BUG, GRASS}, 1, 2),
                         Pokemon("5", {GHOST}, 1, 2)};
  // 0 < 1
  ASSERT_TRUE(pokemons[0] <= pokemons[1]);
  ASSERT_TRUE(pokemons[0] < pokemons[1]);
  ASSERT_TRUE(pokemons[0] != pokemons[1]);
  ASSERT_FALSE(pokemons[0] > pokemons[1]);
  ASSERT_FALSE(pokemons[0] >= pokemons[1]);
  ASSERT_FALSE(pokemons[0] == pokemons[1]);

  // 1 < 2
  ASSERT_TRUE(pokemons[1] <= pokemons[2]);
  ASSERT_TRUE(pokemons[1] < pokemons[2]);
  ASSERT_TRUE(pokemons[1] != pokemons[2]);
  ASSERT_FALSE(pokemons[1] > pokemons[2]);
  ASSERT_FALSE(pokemons[1] >= pokemons[2]);
  ASSERT_FALSE(pokemons[1] == pokemons[2]);

  // 2 < 3
  ASSERT_TRUE(pokemons[2] <= pokemons[3]);
  ASSERT_TRUE(pokemons[2] < pokemons[3]);
  ASSERT_TRUE(pokemons[2] != pokemons[3]);
  ASSERT_FALSE(pokemons[2] > pokemons[3]);
  ASSERT_FALSE(pokemons[2] >= pokemons[3]);
  ASSERT_FALSE(pokemons[2] == pokemons[3]);

  // 3 == 4
  ASSERT_TRUE(pokemons[3] <= pokemons[4]);
  ASSERT_FALSE(pokemons[3] < pokemons[4]);
  ASSERT_FALSE(pokemons[3] != pokemons[4]);
  ASSERT_FALSE(pokemons[3] > pokemons[4]);
  ASSERT_TRUE(pokemons[3] >= pokemons[4]);
  ASSERT_TRUE(pokemons[3] == pokemons[4]);

  // 4 < 5
  ASSERT_TRUE(pokemons[4] <= pokemons[5]);
  ASSERT_TRUE(pokemons[4] < pokemons[5]);
  ASSERT_TRUE(pokemons[4] != pokemons[5]);
  ASSERT_FALSE(pokemons[4] > pokemons[5]);
  ASSERT_FALSE(pokemons[4] >= pokemons[5]);
  ASSERT_FALSE(pokemons[4] == pokemons[5]);

  // 2 < 5
  ASSERT_FALSE(pokemons[5] <= pokemons[2]);
  ASSERT_FALSE(pokemons[5] < pokemons[2]);
  ASSERT_TRUE(pokemons[5] != pokemons[2]);
  ASSERT_TRUE(pokemons[5] > pokemons[2]);
  ASSERT_TRUE(pokemons[5] >= pokemons[2]);
  ASSERT_FALSE(pokemons[5] == pokemons[2]);

  // 1 < 3
  ASSERT_FALSE(pokemons[3] <= pokemons[1]);
  ASSERT_FALSE(pokemons[3] < pokemons[1]);
  ASSERT_TRUE(pokemons[3] != pokemons[1]);
  ASSERT_TRUE(pokemons[3] > pokemons[1]);
  ASSERT_TRUE(pokemons[3] >= pokemons[1]);
  ASSERT_FALSE(pokemons[3] == pokemons[1]);

  return true;
}

bool TestLevel() {
  Pokemon pikachu("pikachu", {FIRE, FLYING}, 3, 1);
  ASSERT_EQUAL(pikachu.Level(), 1);

  Pokemon charmander("charmander", {FIRE, FLYING}, 3, 7);
  ASSERT_EQUAL(charmander.Level(), 7);

  return true;
}

bool TestHit() {
  ostringstream output;
  Pokemon pikachu("pikachu", {FIRE, FLYING}, 1.234, 7);
  Pokemon charmander("charmander", {FIRE, FLYING}, 2.586, 19);
  pikachu.Hit(charmander);
  charmander.Hit(pikachu);
  
  output << pikachu << endl;
  output << charmander << endl;
  
  charmander.Hit(pikachu);
  charmander.Hit(pikachu);

  output << pikachu << endl;
  output << charmander << endl;

  return VerifyOutput(output,
                      "pikachu(7/1.234/50.866) FIRE FLYING\n"
                      "charmander(19/2.586/91.362) FIRE FLYING\n"
                      "pikachu(7/1.234/0) FIRE FLYING\n"
                      "charmander(19/2.586/91.362) FIRE FLYING\n");
}

bool TestHeal() {
  ostringstream output;
  Pokemon pikachu("pikachu", {FIRE, FLYING}, 1.234, 7);
  Pokemon charmander("charmander", {FIRE, FLYING}, 2.586, 19);

  pikachu.Heal();
  charmander.Heal();
  output << pikachu << endl;
  output << charmander << endl;

  pikachu.Hit(charmander);
  charmander.Hit(pikachu);

  pikachu.Heal();
  charmander.Heal();
  output << pikachu << endl;
  output << charmander << endl;

  pikachu.Hit(charmander);
  charmander.Hit(pikachu);
  charmander.Hit(pikachu);
  charmander.Hit(pikachu);

  pikachu.Heal();
  charmander.Heal();
  output << pikachu << endl;
  output << charmander << endl;

  return VerifyOutput(output,
                      "pikachu(7/1.234/100) FIRE FLYING\n"
                      "charmander(19/2.586/100) FIRE FLYING\n"
                      "pikachu(7/1.234/100) FIRE FLYING\n"
                      "charmander(19/2.586/100) FIRE FLYING\n"
                      "pikachu(7/1.234/100) FIRE FLYING\n"
                      "charmander(19/2.586/100) FIRE FLYING\n");
}

bool TestTrain() {
  ostringstream output;

  Pokemon pikachu("pikachu", {FIRE, FLYING}, 1.234, 7);

  ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(-1));
  ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(1));
  ASSERT_THROW(PokemonInvalidArgsException, pikachu.Train(0.5));

  ASSERT_NO_THROW(pikachu.Train(1.7));
  
  output << pikachu << endl;

  return VerifyOutput(output,
                      "pikachu(7/2.0978/100) FIRE FLYING\n");
}

#endif