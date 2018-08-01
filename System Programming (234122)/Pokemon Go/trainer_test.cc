#ifdef T
#include "trainer.h"

#include <iostream>

#include "exceptions.h"
#include "pokemon.h"
#include "test_utils.h"

using namespace std;
using namespace mtm::pokemongo;

namespace {
bool VerifyOutput(const ostringstream& output, const string& expected_output) {
  ASSERT_EQUAL(expected_output, output.str());
  return true;
}
}

bool TestGetStrongestPokemon() {
  ostringstream output;

  Pokemon a("a", {}, 1, 1);
  Pokemon b("b", {ROCK}, 1, 1);
  Pokemon c("c", {FAIRY, GROUND}, 1, 1);
  Pokemon d("d", {BUG, GRASS}, 1, 1);
  Pokemon e("e", {GHOST}, 1, 1);

  Trainer trainer_1("1", BLUE);

  ASSERT_TRUE(trainer_1.TryToCatch(a));
  output << trainer_1.GetStrongestPokemon() << endl;
  output << trainer_1.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_1.TryToCatch(b));
  output << trainer_1.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_1.TryToCatch(c));
  output << trainer_1.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_1.TryToCatch(d));
  output << trainer_1.GetStrongestPokemon() << endl;
  
  ASSERT_TRUE(trainer_1.TryToCatch(e));
  output << trainer_1.GetStrongestPokemon() << endl;

  Trainer trainer_2("2", BLUE);

  ASSERT_TRUE(trainer_2.TryToCatch(b));
  output << trainer_2.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_2.TryToCatch(c));
  output << trainer_2.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_2.TryToCatch(d));
  output << trainer_2.GetStrongestPokemon() << endl;

  ASSERT_TRUE(trainer_2.TryToCatch(a));
  output << trainer_2.GetStrongestPokemon() << endl;
  
  ASSERT_TRUE(trainer_2.TryToCatch(e));
  output << trainer_2.GetStrongestPokemon() << endl;

  return VerifyOutput(output, "a(1/1/100)\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "e(1/1/100) GHOST\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "e(1/1/100) GHOST\n");
}

bool TestKillStrongestPokemon() {
  ostringstream output;

  Pokemon a("a", {}, 1, 1);
  Pokemon b("b", {ROCK}, 1, 1);
  Pokemon c("c", {FAIRY, GROUND}, 1, 1);
  Pokemon d("d", {BUG, GRASS}, 1, 1);
  Pokemon e("e", {GHOST}, 1, 1);

  Trainer trainer("1", BLUE);
  ASSERT_THROW(TrainerNoPokemonsFoundException, trainer.KillStrongestPokemon());

  ASSERT_TRUE(trainer.TryToCatch(a));
  trainer.KillStrongestPokemon();
  output << trainer;
  ASSERT_THROW(TrainerNoPokemonsFoundException, trainer.KillStrongestPokemon());
  ASSERT_TRUE(trainer.TryToCatch(a));

  ASSERT_TRUE(trainer.TryToCatch(b));
  trainer.KillStrongestPokemon();
  output << trainer;
  trainer.KillStrongestPokemon();
  output << trainer;
  ASSERT_THROW(TrainerNoPokemonsFoundException, trainer.KillStrongestPokemon());
  ASSERT_TRUE(trainer.TryToCatch(a));
  ASSERT_TRUE(trainer.TryToCatch(b));

  ASSERT_TRUE(trainer.TryToCatch(c));
  trainer.KillStrongestPokemon();
  output << trainer;
  ASSERT_TRUE(trainer.TryToCatch(c));

  ASSERT_TRUE(trainer.TryToCatch(d));
  trainer.KillStrongestPokemon();
  output << trainer;
  ASSERT_TRUE(trainer.TryToCatch(c));
  
  ASSERT_TRUE(trainer.TryToCatch(e));
  trainer.KillStrongestPokemon();
  output << trainer;
  trainer.KillStrongestPokemon();
  output << trainer;
  trainer.KillStrongestPokemon();
  output << trainer;
  trainer.KillStrongestPokemon();
  output << trainer;
  trainer.KillStrongestPokemon();
  output << trainer;
  ASSERT_THROW(TrainerNoPokemonsFoundException, trainer.KillStrongestPokemon());

  return VerifyOutput(output, "1 (1) BLUE\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "1 (1) BLUE\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "d(1/1/100) BUG GRASS\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "d(1/1/100) BUG GRASS\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "1 (1) BLUE\n"
                              "a(1/1/100)\n"
                              "1 (1) BLUE\n"); 
}

bool TestIsAlly() {
  Trainer red_1("red", RED);
  Trainer blue_1("blue", BLUE);
  Trainer yellow_1("yellow", YELLOW);
  Trainer red_2("red", RED);
  Trainer blue_2("blue", BLUE);
  Trainer yellow_2("yellow", YELLOW);

  ASSERT_TRUE(red_1.IsAlly(red_2));
  ASSERT_TRUE(red_2.IsAlly(red_1));
  ASSERT_TRUE(blue_1.IsAlly(blue_2));
  ASSERT_TRUE(blue_2.IsAlly(blue_1));
  ASSERT_TRUE(yellow_1.IsAlly(yellow_2));
  ASSERT_TRUE(yellow_2.IsAlly(yellow_1));
  ASSERT_FALSE(red_1.IsAlly(yellow_1));
  ASSERT_FALSE(yellow_1.IsAlly(red_1));
  ASSERT_FALSE(blue_1.IsAlly(yellow_1));
  ASSERT_FALSE(yellow_1.IsAlly(blue_1));
  ASSERT_FALSE(blue_1.IsAlly(red_1));
  ASSERT_FALSE(red_1.IsAlly(blue_1));

  return true;
}

bool TestGetTeam() {
  Trainer red("red", RED);
  Trainer blue("blue", BLUE);
  Trainer yellow("yellow", YELLOW);

  ASSERT_EQUAL(red.GetTeam(), RED);
  ASSERT_EQUAL(blue.GetTeam(), BLUE);
  ASSERT_EQUAL(yellow.GetTeam(), YELLOW);

  return true;
}

bool TestTryToCatch() {
  ostringstream output;

  Pokemon a("a", {}, 1, 1);
  Pokemon b("b", {ROCK}, 1, 1);
  Pokemon c("c", {FAIRY, GROUND}, 1, 1);
  Pokemon d("d", {BUG, GRASS}, 1, 1);
  Pokemon e("e", {GHOST}, 1, 1);
  Pokemon pikachu("pikachu", 1, 2);

  Trainer trainer("trainer", BLUE);
  output << trainer;
  
  ASSERT_FALSE(trainer.TryToCatch(pikachu));
  output << trainer;

  ASSERT_TRUE(trainer.TryToCatch(a));
  output << trainer;

  ASSERT_TRUE(trainer.TryToCatch(b));
  output << trainer;

  ASSERT_TRUE(trainer.TryToCatch(c));
  output << trainer;

  ASSERT_TRUE(trainer.TryToCatch(d));
  output << trainer;
  
  ASSERT_TRUE(trainer.TryToCatch(e));
  output << trainer;

  ASSERT_FALSE(trainer.TryToCatch(pikachu));
  output << trainer;
  
  return VerifyOutput(output, "trainer (1) BLUE\n"
                              "trainer (1) BLUE\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "d(1/1/100) BUG GRASS\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "d(1/1/100) BUG GRASS\n"
                              "e(1/1/100) GHOST\n"
                              "trainer (1) BLUE\n"
                              "a(1/1/100)\n"
                              "b(1/1/100) ROCK\n"
                              "c(1/1/100) FAIRY GROUND\n"
                              "d(1/1/100) BUG GRASS\n"
                              "e(1/1/100) GHOST\n");
}

#endif