#define BOOST_TEST_MODULE Graph   // specify the name of your test module
#include <boost/test/included/unit_test.hpp>  // include this to get main()
#include "Graph.cpp"
#include <sstream>
#include <iostream>


BOOST_AUTO_TEST_CASE(test_init_graph)
{
  Graph g;
  g.init_graph();
  BOOST_CHECK(nullptr == g.vertices);
}

BOOST_AUTO_TEST_CASE(test_init_graph_file)
{
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(nullptr != g.vertices);
}

BOOST_AUTO_TEST_CASE(test_add_vertex)
{
  Graph g;
  g.init_graph();
  g.add_vertex("V");
  BOOST_CHECK(true == g.contains_vertex("V"));
  BOOST_CHECK(false == g.contains_vertex("V2"));
}

BOOST_AUTO_TEST_CASE(test_add_edge)
{
  Graph g;
  g.init_graph();
  g.add_vertex("V1");
  g.add_vertex("V2");
  g.add_edge("V1","V2",2);
  BOOST_CHECK(true == g.contains_edge("V1","V2"));
  BOOST_CHECK(false == g.contains_edge("V2","V1"));
}


BOOST_AUTO_TEST_CASE(test_contains_vertex)
{
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(true == g.contains_vertex("A"));
  BOOST_CHECK(true == g.contains_vertex("B"));
  BOOST_CHECK(true == g.contains_vertex("C"));
  BOOST_CHECK(true == g.contains_vertex("D"));
  BOOST_CHECK(true == g.contains_vertex("E"));
  BOOST_CHECK(true == g.contains_vertex("F"));
  BOOST_CHECK(true == g.contains_vertex("G"));
  BOOST_CHECK(true == g.contains_vertex("S"));
  BOOST_CHECK(true == g.contains_vertex("T"));
  BOOST_CHECK(false == g.contains_vertex("H"));
  BOOST_CHECK(false == g.contains_vertex("W"));
}

BOOST_AUTO_TEST_CASE(test_contains_edge) {
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(true == g.contains_edge("C", "E"));
  BOOST_CHECK(true == g.contains_edge("E", "G"));
  BOOST_CHECK(true == g.contains_edge("S", "D"));
  BOOST_CHECK(false == g.contains_edge("T", "G"));
  BOOST_CHECK(false == g.contains_edge("D", "S"));
  BOOST_CHECK(true == g.contains_edge("S", "B"));
  BOOST_CHECK(true == g.contains_edge("B", "S"));
}

BOOST_AUTO_TEST_CASE(test_get_distance_1)
{
  Graph g;
  g.init_graph();
  g.add_vertex("V1");
  g.add_vertex("V2");
  g.add_edge("V1","V2",2);
  BOOST_CHECK(2 == g.get_distance("V1","V2"));
  BOOST_CHECK(-1 == g.get_distance("V2","V1"));
}

BOOST_AUTO_TEST_CASE(test_get_distance_2)
{
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(7 == g.get_distance("S","D"));
  BOOST_CHECK(-1 == g.get_distance("F","T"));
  BOOST_CHECK(3 == g.get_distance("G","T"));
}

BOOST_AUTO_TEST_CASE(test_get_Vertex)
{
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(nullptr == g.getVertex("X"));
  BOOST_CHECK(nullptr == g.getVertex("H"));
  BOOST_CHECK("A" == g.getVertex("A")->name);
  BOOST_CHECK("S" == g.getVertex("S")->name);
}

BOOST_AUTO_TEST_CASE(test_dijkstra)
{
  Graph g;
  g.init_graph("test_graph");
  BOOST_CHECK(0 == g.apply_dijkstra("S","S"));
  BOOST_CHECK(1 == g.apply_dijkstra("A","C"));
  BOOST_CHECK(8 == g.apply_dijkstra("S","G"));
  BOOST_CHECK(-1 == g.apply_dijkstra("S","X"));
}