#ifdef PG
#include "pokemon_go.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "test_utils.h"
#include "trainer.h"
#include "world.h"

using namespace std;
using namespace mtm;
using namespace mtm::pokemongo;

namespace {

bool VerifyOutput(const ostringstream& output, const string& expected_output) {
  ASSERT_EQUAL(expected_output, output.str());
  return true;
}

void SetUpWorld(World* world, string filename) {
  ifstream world_in(filename);
  string line;

  while (std::getline(world_in, line)) {
      istringstream line_stream(line);
      line_stream >> *world;
  }

  world->Connect("a", "b", EAST, WEST);
  world->Connect("a", "c", NORTH, SOUTH);
  world->Connect("b", "d", NORTH, SOUTH);
  world->Connect("b", "e", EAST, SOUTH);
  world->Connect("c", "d", EAST, WEST);
  world->Connect("d", "e", EAST, WEST);
  world->Connect("d", "f", NORTH, WEST);
  world->Connect("e", "f", NORTH, SOUTH);
}
}
bool TestAddTrainerExceptions() {
  World* world = new World();
  SetUpWorld(world, "empty_world.in");
  PokemonGo pokemon_go(world);

  ASSERT_THROW(PokemonGoInvalidArgsException, pokemon_go.AddTrainer("", RED, "a"));
  ASSERT_THROW(PokemonGoInvalidArgsException, pokemon_go.AddTrainer("", RED, "n"));
  ASSERT_THROW(PokemonGoLocationNotFoundException, pokemon_go.AddTrainer("name", RED, "n"));
  
  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", BLUE, "a"));
  ASSERT_THROW(PokemonGoTrainerNameAlreadyUsedExcpetion, pokemon_go.AddTrainer("1", RED, "b"));
  ASSERT_THROW(PokemonGoTrainerNameAlreadyUsedExcpetion, pokemon_go.AddTrainer("1", RED, "n"));

  return true;
}

bool TestWhereIsExceptions() {
  World* world = new World();
  SetUpWorld(world, "empty_world.in");
  PokemonGo pokemon_go(world);

  ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion, pokemon_go.WhereIs(""));
  ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion, pokemon_go.WhereIs("1"));
  
  return true;
}

bool TestGetTrainersInExceptions() {
  World* world = new World();
  SetUpWorld(world, "empty_world.in");
  PokemonGo pokemon_go(world);

  ASSERT_THROW(PokemonGoLocationNotFoundException, pokemon_go.GetTrainersIn("n"));

  return true;
}

bool TestMoveTrainerExceptions() {
  World* world = new World();
  SetUpWorld(world, "empty_world.in");
  PokemonGo pokemon_go(world);

  ASSERT_THROW(PokemonGoTrainerNotFoundExcpetion, pokemon_go.MoveTrainer("1", SOUTH));
  
  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", BLUE, "a"));
  ASSERT_THROW(PokemonGoReachedDeadEndException, pokemon_go.MoveTrainer("1", SOUTH));

  return true;
}

bool TestPokemonGo0() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "empty_world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", BLUE, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", BLUE, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  ASSERT_EQUAL(pokemon_go.WhereIs("1"), "a");
  ASSERT_EQUAL(pokemon_go.WhereIs("2"), "a");
  ASSERT_EQUAL(pokemon_go.WhereIs("3"), "b");
  ASSERT_EQUAL(pokemon_go.WhereIs("4"), "d");
  ASSERT_EQUAL(pokemon_go.WhereIs("5"), "e");

  const vector<Trainer*>& trainers_a = pokemon_go.GetTrainersIn("a");
  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_c = pokemon_go.GetTrainersIn("c");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");

  ASSERT_EQUAL(trainers_a.size(), 2);
  ASSERT_EQUAL(trainers_b.size(), 1);
  ASSERT_EQUAL(trainers_c.size(), 0);
  ASSERT_EQUAL(trainers_d.size(), 1);
  ASSERT_EQUAL(trainers_e.size(), 1);
  ASSERT_EQUAL(trainers_f.size(), 0);

  output << *trainers_a[0];
  output << *trainers_a[1];
  output << *trainers_b[0];
  output << *trainers_d[0];
  output << *trainers_e[0];
  
  pokemon_go.MoveTrainer("1", NORTH);
  ASSERT_EQUAL(trainers_a.size(), 1);
  ASSERT_EQUAL(trainers_c.size(), 1);
  output << *trainers_a[0];
  output << *trainers_c[0]; 

  pokemon_go.MoveTrainer("4", WEST);
  ASSERT_EQUAL(trainers_d.size(), 0);
  ASSERT_EQUAL(trainers_c.size(), 2);
  output << *trainers_c[0];
  output << *trainers_c[1];
  
  pokemon_go.MoveTrainer("3", EAST);
  ASSERT_EQUAL(trainers_b.size(), 0);
  ASSERT_EQUAL(trainers_e.size(), 2);
  output << *trainers_e[0];
  output << *trainers_e[1];

  pokemon_go.MoveTrainer("3", NORTH);
  ASSERT_EQUAL(trainers_f.size(), 1);
  ASSERT_EQUAL(trainers_e.size(), 1);
  output << *trainers_f[0];
  output << *trainers_e[0];

  pokemon_go.MoveTrainer("1", EAST);
  pokemon_go.MoveTrainer("3", WEST);
  ASSERT_EQUAL(trainers_f.size(), 0);
  ASSERT_EQUAL(trainers_c.size(), 1);
  ASSERT_EQUAL(trainers_d.size(), 2);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_c[0];

  return VerifyOutput(output, "1 (1) BLUE\n"
                              "2 (1) BLUE\n"
                              "3 (1) BLUE\n"
                              "4 (1) BLUE\n"
                              "5 (1) BLUE\n"
                              "2 (1) BLUE\n"
                              "1 (1) BLUE\n"
                              "1 (1) BLUE\n"
                              "4 (1) BLUE\n"
                              "5 (1) BLUE\n"
                              "3 (1) BLUE\n"
                              "3 (1) BLUE\n"
                              "5 (1) BLUE\n"
                              "1 (1) BLUE\n"
                              "3 (1) BLUE\n"
                              "4 (1) BLUE\n");
}

bool TestPokemonGo1() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n");
}

bool TestPokemonGo2() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("3", NORTH);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_f[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 27);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_f[0];
  output << *trainers_e[0];
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 10);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "3 (1) RED\n"
                              "A(1/2.5/97.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "4 (1) YELLOW\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/100) NORMAL BUG FAIRY GROUND\n");
}

bool TestPokemonGo3() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("3", NORTH);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_f[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 27);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_f[0];
  output << *trainers_e[0];
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 10);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);
  
  pokemon_go.MoveTrainer("4", SOUTH);
  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("4", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 13);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 30);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 2);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "3 (1) RED\n"
                              "A(1/2.5/97.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "4 (1) YELLOW\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/100) NORMAL BUG FAIRY GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/95.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/91.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "5 (3) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/97.5) NORMAL BUG FAIRY GROUND\n");
}

bool TestPokemonGo4() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_c = pokemon_go.GetTrainersIn("c");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("3", NORTH);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_f[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 27);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_f[0];
  output << *trainers_e[0];
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 10);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);
  
  pokemon_go.MoveTrainer("4", SOUTH);
  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("4", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 13);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 30);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 2);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", EAST);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 34);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 3);

  pokemon_go.MoveTrainer("1", NORTH);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_c[0];
  output << *trainers_c[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 24);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 9);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "3 (1) RED\n"
                              "A(1/2.5/97.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "4 (1) YELLOW\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/100) NORMAL BUG FAIRY GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/95.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/91.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "5 (3) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/97.5) NORMAL BUG FAIRY GROUND\n"
                              "3 (3) RED\n"
                              "A(1/2.5/84.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/95) NORMAL GROUND\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/100) NORMAL ROCK\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/100) ROCK BUG FAIRY\n");
}

bool TestPokemonGo5() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_c = pokemon_go.GetTrainersIn("c");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("3", NORTH);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_f[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 27);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_f[0];
  output << *trainers_e[0];
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 10);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);
  
  pokemon_go.MoveTrainer("4", SOUTH);
  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("4", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 13);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 30);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 2);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", EAST);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 34);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 3);

  pokemon_go.MoveTrainer("1", NORTH);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_c[0];
  output << *trainers_c[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 24);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 9);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  pokemon_go.MoveTrainer("3", WEST);
  pokemon_go.MoveTrainer("2", NORTH);
  output << *trainers_c[0];
  output << *trainers_c[1];
  output << *trainers_c[2];
  output << *trainers_c[3];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 17);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("1", EAST);
  pokemon_go.MoveTrainer("5", EAST);
  pokemon_go.MoveTrainer("3", EAST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 31);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -3);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "3 (1) RED\n"
                              "A(1/2.5/97.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "4 (1) YELLOW\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/100) NORMAL BUG FAIRY GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/95.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/91.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "5 (3) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/97.5) NORMAL BUG FAIRY GROUND\n"
                              "3 (3) RED\n"
                              "A(1/2.5/84.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/95) NORMAL GROUND\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/100) NORMAL ROCK\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/100) ROCK BUG FAIRY\n"
                              "1 (1) RED\n"
                              "G(1/4/100) NORMAL ROCK\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/100) ROCK BUG FAIRY\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/100) NORMAL ROCK BUG FAIRY GROUND\n"
                              "2 (1) BLUE\n"
                              "J(1/2/100) NORMAL ROCK BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/84) NORMAL ROCK\n"
                              "5 (8) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/89.6) ROCK BUG FAIRY\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/84) NORMAL ROCK BUG FAIRY GROUND\n");
}


bool TestPokemonGo6() {
  ostringstream output;
  World* world = new World();
  SetUpWorld(world, "world.in");
  PokemonGo pokemon_go(world);

  ASSERT_NO_THROW(pokemon_go.AddTrainer("1", RED, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("2", BLUE, "a"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("3", RED, "b"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("4", YELLOW, "d"));
  ASSERT_NO_THROW(pokemon_go.AddTrainer("5", BLUE, "e"));

  const vector<Trainer*>& trainers_b = pokemon_go.GetTrainersIn("b");
  const vector<Trainer*>& trainers_c = pokemon_go.GetTrainersIn("c");
  const vector<Trainer*>& trainers_d = pokemon_go.GetTrainersIn("d");
  const vector<Trainer*>& trainers_e = pokemon_go.GetTrainersIn("e");
  const vector<Trainer*>& trainers_f = pokemon_go.GetTrainersIn("f");
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  output << *trainers_b[0];

  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_b[0];
  output << *trainers_b[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 11);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("3", NORTH);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_f[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 27);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);

  pokemon_go.MoveTrainer("5", NORTH);
  pokemon_go.MoveTrainer("5", SOUTH);
  output << *trainers_f[0];
  output << *trainers_e[0];
  
  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 10);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 1);
  
  pokemon_go.MoveTrainer("4", SOUTH);
  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("4", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 17);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 13);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 30);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 2);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 0);

  pokemon_go.MoveTrainer("5", EAST);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 34);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 0);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 3);

  pokemon_go.MoveTrainer("1", NORTH);
  pokemon_go.MoveTrainer("5", WEST);
  output << *trainers_c[0];
  output << *trainers_c[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 24);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), 9);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  pokemon_go.MoveTrainer("3", WEST);
  pokemon_go.MoveTrainer("2", NORTH);
  output << *trainers_c[0];
  output << *trainers_c[1];
  output << *trainers_c[2];
  output << *trainers_c[3];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 14);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -1);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 17);

  pokemon_go.MoveTrainer("4", NORTH);
  pokemon_go.MoveTrainer("1", EAST);
  pokemon_go.MoveTrainer("5", EAST);
  pokemon_go.MoveTrainer("3", EAST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_d[2];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 31);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -3);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  pokemon_go.MoveTrainer("3", EAST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_e[0];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 31);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -3);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  pokemon_go.MoveTrainer("2", EAST);
  pokemon_go.MoveTrainer("5", EAST);
  output << *trainers_d[0];
  output << *trainers_d[1];
  output << *trainers_e[0];
  output << *trainers_e[1];

  ASSERT_EQUAL(pokemon_go.GetScore(BLUE), 32);
  ASSERT_EQUAL(pokemon_go.GetScore(RED), -3);
  ASSERT_EQUAL(pokemon_go.GetScore(YELLOW), 7);

  return VerifyOutput(output, "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "3 (1) RED\n"
                              "A(1/2.5/100) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "5 (1) BLUE\n"
                              "B(1/2.3/100) NORMAL ROCK BUG GHOST\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "3 (1) RED\n"
                              "A(1/2.5/97.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "4 (1) YELLOW\n"
                              "5 (2) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/100) NORMAL BUG FAIRY GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/95.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "3 (2) RED\n"
                              "A(1/2.5/91.47) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/97.5) NORMAL GROUND\n"
                              "5 (3) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2/97.5) NORMAL BUG FAIRY GROUND\n"
                              "3 (3) RED\n"
                              "A(1/2.5/84.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "4 (1) YELLOW\n"
                              "L(1/2/95) NORMAL GROUND\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/100) NORMAL ROCK\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/100) ROCK BUG FAIRY\n"
                              "1 (1) RED\n"
                              "G(1/4/100) NORMAL ROCK\n"
                              "5 (5) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/100) ROCK BUG FAIRY\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/100) NORMAL ROCK BUG FAIRY GROUND\n"
                              "2 (1) BLUE\n"
                              "J(1/2/100) NORMAL ROCK BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/84) NORMAL ROCK\n"
                              "5 (8) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/89.6) ROCK BUG FAIRY\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/84) NORMAL ROCK BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/84) NORMAL ROCK\n"
                              "5 (8) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/89.6) ROCK BUG FAIRY\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/84) NORMAL ROCK BUG FAIRY GROUND\n"
                              "1 (1) RED\n"
                              "G(1/4/84) NORMAL ROCK\n"
                              "2 (1) BLUE\n"
                              "J(1/2/100) NORMAL ROCK BUG FAIRY GROUND\n"
                              "3 (4) RED\n"
                              "A(1/2.5/82.67) NORMAL ROCK BUG GROUND ICE GHOST PSYCHIC\n"
                              "I(2/3.2/84) NORMAL ROCK BUG FAIRY GROUND\n"
                              "5 (8) BLUE\n"
                              "B(1/2.53/97.5) NORMAL ROCK BUG GHOST\n"
                              "K(2/2.4/95) NORMAL BUG FAIRY GROUND\n"
                              "H(4/4/89.6) ROCK BUG FAIRY\n");
}
#endif