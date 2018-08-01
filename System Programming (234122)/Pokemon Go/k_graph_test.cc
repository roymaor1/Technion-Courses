#ifdef G
#include "k_graph_mtm.h"

#include <iostream>
#include <string>

#include "location.h"
#include "test_utils.h"

using namespace std;
using namespace mtm;
using namespace mtm::pokemongo;

typedef KGraph<string, Location*, 4> LGraph;

Location* locations[5];

namespace {
LGraph* SetUp() {
  locations[0] = new Location();
  locations[1] = new Location();
  locations[2] = new Location();
  locations[3] = new Location();
  locations[4] = new Location();

  LGraph* graph_ptr = new LGraph(nullptr);

  graph_ptr->Insert("A", locations[0]);
  graph_ptr->Insert("B", locations[1]);
  graph_ptr->Insert("C", locations[2]);
  graph_ptr->Insert("D", locations[3]);
  graph_ptr->Insert("E", locations[4]);
  graph_ptr->Insert("NULL", locations[4]);

  return graph_ptr;
}

void TearDown(LGraph* graph_ptr) {
  for (int i  = 0; i < 5; ++i) {
    delete locations[i];
  }
  delete graph_ptr;
}
}

bool TestOperatorSubscript() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;
  const LGraph& const_graph = *graph_ptr;

  ASSERT_EQUAL(graph["A"], locations[0]);
  ASSERT_EQUAL(graph["B"], locations[1]);
  ASSERT_EQUAL(graph["C"], locations[2]);
  ASSERT_EQUAL(graph["D"], locations[3]);
  ASSERT_EQUAL(graph["E"], locations[4]);

  ASSERT_EQUAL(const_graph["A"], locations[0]);
  ASSERT_EQUAL(const_graph["B"], locations[1]);
  ASSERT_EQUAL(const_graph["C"], locations[2]);
  ASSERT_EQUAL(const_graph["D"], locations[3]);
  ASSERT_EQUAL(const_graph["E"], locations[4]);


  ASSERT_THROW(KGraphKeyNotFoundException, const_graph["F"]);
  ASSERT_EQUAL(graph["F"], nullptr);

  Location* location_f = new Location();
  graph["F"] = location_f;

  ASSERT_EQUAL(graph["F"], location_f);
  
  delete location_f;
  TearDown(graph_ptr);
  return true;
}

bool TestContains() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;
  
  ASSERT_TRUE(graph.Contains("A"));
  ASSERT_TRUE(graph.Contains("B"));
  ASSERT_TRUE(graph.Contains("C"));
  ASSERT_TRUE(graph.Contains("D"));
  ASSERT_TRUE(graph.Contains("E"));
  ASSERT_TRUE(graph.Contains("NULL"));
  ASSERT_FALSE(graph.Contains("F"));

  TearDown(graph_ptr);
  return true;
}

bool TestInsert() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  ASSERT_THROW(KGraphKeyAlreadyExistsExpection,
               graph.Insert("A", locations[0]));
  ASSERT_THROW(KGraphKeyAlreadyExistsExpection,
               graph.Insert("NULL", locations[0]));

  ASSERT_NO_THROW(graph.Insert("F", locations[0]));
  ASSERT_NO_THROW(graph.Insert("G", locations[0]));
  ASSERT_NO_THROW(graph.Insert("DEFAULT"));
  
  ASSERT_EQUAL(graph["F"], locations[0]);
  ASSERT_EQUAL(graph["G"], locations[0]);
  ASSERT_EQUAL(graph["DEFAULT"], nullptr);
  
  TearDown(graph_ptr);
  return true;
}

bool TestRemove() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;
  const LGraph& const_graph = *graph_ptr;
  graph.Connect("A", "NULL", 0, 1);
  graph.Connect("B", "NULL", 0, 2);
  graph.Connect("C", "NULL", 0, 3);
  graph.Connect("NULL", 0);

  ASSERT_THROW(KGraphKeyNotFoundException, graph.Remove("F"));

  ASSERT_TRUE(graph.Contains("A"));
  ASSERT_NO_THROW(graph.Remove("A"));
  ASSERT_FALSE(graph.Contains("A"));
  ASSERT_THROW(KGraphKeyNotFoundException, const_graph["A"]);

  ASSERT_NO_THROW(graph.Remove(graph.BeginAt("B")));return true;

  ASSERT_FALSE(graph.Contains("B"));

  ASSERT_THROW(KGraphKeyNotFoundException, const_graph["B"]);

  LGraph::iterator i = graph.BeginAt("NULL");
  LGraph::const_iterator end = graph.End();
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");
  ASSERT_NO_THROW(i.Move(1));
  ASSERT_EQUAL(i, end);
  i = graph.BeginAt("NULL");
  ASSERT_NO_THROW(i.Move(2));
  ASSERT_EQUAL(i, end);
  i = graph.BeginAt("NULL");
  ASSERT_NO_THROW(i.Move(3));
  ASSERT_NOT_EQUAL(i, end);

  TearDown(graph_ptr);
  return true;
}

bool TestIterators() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  graph.Connect("A", "NULL", 0, 1);
  graph.Connect("B", "NULL", 0, 2);
  graph.Connect("C", "NULL", 0, 3);
  graph.Connect("A", "B", 1, 1);
  graph.Connect("B", "D", 2, 2);
  graph.Connect("NULL", 0);

  LGraph::iterator i = graph.BeginAt("NULL");
  LGraph::const_iterator end = graph.End();
  ASSERT_NOT_EQUAL(i, end);
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(7));
  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(-2));

  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(1));
  ASSERT_EQUAL(*i, "A");
  ASSERT_EQUAL(i, graph.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(2));
  ASSERT_EQUAL(*i, "B");
  ASSERT_NOT_EQUAL(i, graph.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(3));
  ASSERT_EQUAL(*i, "C");
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(1).Move(2));
  ASSERT_EQUAL(i, end);
  ASSERT_THROW(KGraphIteratorReachedEnd, i.Move(1));
  ASSERT_THROW(KGraphIteratorReachedEnd, *i);

  TearDown(graph_ptr);
  return true;
}

bool TestConstIterators() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  graph.Connect("A", "NULL", 0, 1);
  graph.Connect("B", "NULL", 0, 2);
  graph.Connect("C", "NULL", 0, 3);
  graph.Connect("NULL", 0);

  ASSERT_THROW(KGraphKeyNotFoundException, graph.BeginAt("F"));

  LGraph::const_iterator i = graph.BeginAt("NULL");
  LGraph::const_iterator end = graph.End();
  ASSERT_NOT_EQUAL(i, end);
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(7));
  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(-2));

  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(1));
  ASSERT_EQUAL(*i, "A");
  ASSERT_EQUAL(i, graph.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(2));
  ASSERT_EQUAL(*i, "B");
  ASSERT_NOT_EQUAL(i, graph.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(3));
  ASSERT_EQUAL(*i, "C");
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(1).Move(2));
  ASSERT_EQUAL(i, end);
  ASSERT_THROW(KGraphIteratorReachedEnd, i.Move(1));
  ASSERT_THROW(KGraphIteratorReachedEnd, *i);

  TearDown(graph_ptr);
  return true;
}

bool TestConnect() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  ASSERT_NO_THROW(graph.Connect("A", "NULL", 0, 1));
  ASSERT_NO_THROW(graph.Connect("B", "NULL", 0, 2));
  ASSERT_NO_THROW(graph.Connect("NULL", 0));

  ASSERT_THROW(KGraphEdgeAlreadyInUse, graph.Connect("C", "NULL", 0, 1));
  ASSERT_THROW(KGraphEdgeAlreadyInUse, graph.Connect("C", "NULL", 0, 0));
  ASSERT_THROW(KGraphNodesAlreadyConnected, graph.Connect("NULL", 1));
  ASSERT_THROW(KGraphNodesAlreadyConnected, graph.Connect("A", "NULL", 3, 2));

  TearDown(graph_ptr);
  return true;
}

bool TestDisconnect() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  ASSERT_NO_THROW(graph.Connect("A", "NULL", 0, 1));
  ASSERT_NO_THROW(graph.Connect("B", "NULL", 0, 2));
  ASSERT_NO_THROW(graph.Connect("C", "NULL", 0, 3));
  ASSERT_NO_THROW(graph.Connect("NULL", 0));

  ASSERT_NO_THROW(graph.Disconnect("C", "NULL"));
  ASSERT_NO_THROW(graph.Disconnect("NULL", "NULL"));

  ASSERT_THROW(KGraphExcpetion, graph.Disconnect("C", "NULL"));
  ASSERT_THROW(KGraphExcpetion, graph.Disconnect("NULL", "NULL"));

  ASSERT_THROW(KGraphEdgeAlreadyInUse, graph.Connect("C", "NULL", 0, 1));
  ASSERT_THROW(KGraphEdgeAlreadyInUse, graph.Connect("NULL", 1));
  ASSERT_THROW(KGraphNodesAlreadyConnected, graph.Connect("A", "NULL", 3, 2));

  TearDown(graph_ptr);
  return true;
}

bool TestCopy() {
  LGraph* graph_ptr = SetUp();
  LGraph& graph = *graph_ptr;

  graph.Connect("A", "NULL", 0, 1);
  graph.Connect("B", "NULL", 0, 2);
  graph.Connect("C", "NULL", 0, 3);
  graph.Connect("NULL", 0);

  LGraph copy(graph);

  LGraph::iterator i = copy.BeginAt("NULL");

  LGraph::const_iterator end = copy.End();
  ASSERT_NOT_EQUAL(i, end);
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(7));
  ASSERT_THROW(KGraphEdgeOutOfRange, i.Move(-2));

  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");
  ASSERT_EQUAL(*i, "NULL");
  ASSERT_NO_THROW(i.Move(1));
  ASSERT_EQUAL(*i, "A");
  ASSERT_EQUAL(i, copy.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(2));
  ASSERT_EQUAL(*i, "B");
  ASSERT_NOT_EQUAL(i, copy.BeginAt("A"));
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(3));
  ASSERT_EQUAL(*i, "C");
  ASSERT_NO_THROW(i.Move(0));
  ASSERT_EQUAL(*i, "NULL");

  ASSERT_NO_THROW(i.Move(1).Move(2));
  ASSERT_EQUAL(i, end);
  ASSERT_THROW(KGraphIteratorReachedEnd, i.Move(1));
  ASSERT_THROW(KGraphIteratorReachedEnd, *i);
  
  ASSERT_NO_THROW(copy.Remove("A"));
  ASSERT_FALSE(copy.Contains("A"));
  ASSERT_TRUE(graph.Contains("A"));
  
  ASSERT_NO_THROW(copy.Remove(graph.BeginAt("B")));
  ASSERT_FALSE(copy.Contains("B"));
  ASSERT_TRUE(graph.Contains("A"));

  TearDown(graph_ptr);
  return true;
}

#endif