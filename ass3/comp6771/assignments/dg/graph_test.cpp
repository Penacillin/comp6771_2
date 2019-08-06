/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/
#include <iostream>
#include <string>

#include "assignments/dg/graph.h"
#include "catch.h"

SCENARIO("Graphs can be constructed using various methods and it will contain the expected "
        " graph.") {
  WHEN("An empty graph is created") {
    gdwg::Graph<int, int> g;
    THEN("The output of the graph should ''") {
      auto os = std::ostringstream();
      os << g;
      REQUIRE(os.str() == "");
    }
  }

  GIVEN("A vector of 0 nodes") {
    std::vector<int> nodes;
    WHEN("A graph is constructed using this vector") {
      gdwg::Graph<int, int> g(nodes.begin(), nodes.end());
      THEN("The graph will be empty") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "");
      }
    }
  }

  GIVEN("A vector with one node") {
    std::vector<int> nodes{1};
    WHEN("A graph is contrsucted using this vector") {
      gdwg::Graph<int, int> g(nodes.begin(), nodes.end());
      THEN("The graph will have one node with no edge") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "1 (\n)\n");

        AND_WHEN("We add an edge from this one node to itself") {
          g.InsertEdge(1, 1, 2);
          THEN("The graph will have 1 edge") {
            auto os = std::ostringstream();
            os << g;
            REQUIRE(os.str() == "1 (\n  1 | 2\n)\n");
          }
        }
      }
    }
  }

  GIVEN("A vector with two nodes") {
    std::vector<int> nodes{3, 1};
    WHEN("A graph is contrsucted using this vector") {
      gdwg::Graph<int, int> g(nodes.begin(), nodes.end());
      THEN("The graph will have two nodes with no edge") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "1 (\n)\n3 (\n)\n");

        AND_WHEN("We add an edge from 1->1 with weight 2") {
          g.InsertEdge(1, 1, 2);
          THEN("The graph will have 1 edge") {
            auto os = std::ostringstream();
            os << g;
            REQUIRE(os.str() == "1 (\n  1 | 2\n)\n3 (\n)\n");
          }
        }

        AND_WHEN("We add multiples edges (1->1, 1->3, 3->1, 3->3)") {
          g.InsertEdge(1, 1, 2);
          g.InsertEdge(1, 3, 3);
          g.InsertEdge(3, 1, 4);
          g.InsertEdge(3, 3, 5);
          THEN("The graph will have 4 edges") {
            auto os = std::ostringstream();
            os << g;
            REQUIRE(os.str() == "1 (\n  1 | 2\n  3 | 3\n)\n3 (\n  1 | 4\n  3 | 5\n)\n");
          }
        }
      }
    }
  }

  GIVEN("A vector of tuples describing zero edge") {
    auto edge_vector = std::vector<std::tuple<int, int, int>>();

    WHEN("We use this vector of no edges to create a graph") {
      gdwg::Graph<int, int> g(edge_vector.begin(), edge_vector.end());

      THEN("the graph with no edges") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "");
      }
    }
  }

  GIVEN("A vector of tuples describing a few edges") {
    auto e1 = std::make_tuple(1, 2, 3);
    auto e2 = std::make_tuple(3, 4, 4);
    auto e3 = std::make_tuple(2, 1, 4);
    auto edge_vector = std::vector<std::tuple<int, int, int>>{e1, e2, e3};

    WHEN("We use this vector to create a graph") {
      gdwg::Graph<int, int> g(edge_vector.begin(), edge_vector.end());

      THEN("the graph will have the edges described by the tuples") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "1 (\n  2 | 3\n)\n2 (\n  1 | 4\n)\n3 (\n  4 | 4\n)\n4 (\n)\n");
      }

      AND_WHEN("We use the copy constructor to make a new graph") {
        auto g2 = g;
        THEN("These two graphs will have the same graph") {
          auto os = std::ostringstream();
          os << g;
          auto os2 = std::ostringstream();
          os2 << g2;
          REQUIRE(os.str() == os2.str());
        }
      }

      AND_WHEN("We use the move constructor to make a new graph") {
        auto g2 = std::move(g);
        THEN("These new graph will have the original graph and the old graph will have 0 nodes") {
          auto os = std::ostringstream();
          os << g;
          auto os2 = std::ostringstream();
          os2 << g2;
          REQUIRE(os2.str() == "1 (\n  2 | 3\n)\n2 (\n  1 | 4\n)\n3 (\n  4 | 4\n)\n4 (\n)\n");
          REQUIRE(os.str() == "");
        }
      }

      AND_WHEN("We create an empty graph and assign the original graph into it") {
        gdwg::Graph<int, int> g2;
        g2 = g;

        THEN("These two graphs will have the same graph") {
          auto os = std::ostringstream();
          os << g;
          auto os2 = std::ostringstream();
          os2 << g2;
          REQUIRE(os.str() == os2.str());
        }
      }

      AND_WHEN("We create an empty graph and move assign graph into it") {
        gdwg::Graph<int, int> g2;
        g2 = std::move(g);

        THEN("These the original graph will be empty, the new one will have the original one") {
          auto os = std::ostringstream();
          os << g;
          auto os2 = std::ostringstream();
          os2 << g2;
          REQUIRE(os2.str() == "1 (\n  2 | 3\n)\n2 (\n  1 | 4\n)\n3 (\n  4 | 4\n)\n4 (\n)\n");
          REQUIRE(os.str() == "");
        }
      }

      AND_WHEN("We create a non empty graph and move assign graph into it") {
        gdwg::Graph<int, int> g2{1, 2};
        g2 = std::move(g);

        THEN("The new graph will have the original graph, the original graph will be empty") {
          auto os = std::ostringstream();
          os << g;
          auto os2 = std::ostringstream();
          os2 << g2;
          REQUIRE(os2.str() == "1 (\n  2 | 3\n)\n2 (\n  1 | 4\n)\n3 (\n  4 | 4\n)\n4 (\n)\n");
          REQUIRE(os.str() == "");
        }
      }
    }
  }

  GIVEN("An empty initializer list") {
    auto l1 = std::initializer_list<std::string>{};

    WHEN("We use this list to create a graph") {
      gdwg::Graph<std::string, int> g(l1);

      THEN("the graph will have no nodes") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "");
      }
    }
  }

  GIVEN("An initializer list of nodes") {
    auto l1 = std::initializer_list<std::string>{"A", "B"};

    WHEN("We use this list to create a graph") {
      gdwg::Graph<std::string, int> g(l1);

      THEN("the graph will have the two nodes with no edges") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == "A (\n)\nB (\n)\n");
      }
    }
  }
}

SCENARIO("Graphs can be constructed and then can be manipulated using insert/delete/replace"
        " methods") {
  WHEN("A graph with nodes 1, 2, 3 is created") {
    gdwg::Graph<int, int> g{1, 2, 3};

    THEN("1 2 and 3 are nodes") {
      REQUIRE(g.IsNode(1));
      REQUIRE(g.IsNode(2));
      REQUIRE(g.IsNode(3));

      AND_WHEN("We connect each node to itself") {
        g.InsertEdge(1, 1, 1);
        g.InsertEdge(2, 2, 1);
        g.InsertEdge(3, 3, 1);

        THEN("Each node is connected to itself") {
          g.IsConnected(1, 1);
          g.IsConnected(2, 2);
          g.IsConnected(3, 3);
        }

        AND_WHEN("We add more edges between nodes (1->2, 2->1, 3->1)") {
          g.InsertEdge(1, 2, 3);
          g.InsertEdge(2, 1, 3);
          g.InsertEdge(3, 1, 3);

          AND_WHEN("We replace node 1 with an existing node (2)") {
            auto res = g.Replace(1, 2);
            REQUIRE(res == false);
          }

          AND_WHEN("We replace 2 with 5") {
            auto res = g.Replace(2, 5);

            THEN("We expect the replace to return true and the graph to be updated with "
                    " 2 as 5") {
              REQUIRE(res == true);
              auto os = std::ostringstream();
              os << g;
              REQUIRE(os.str() == R"&(1 (
  1 | 1
  5 | 3
)
3 (
  1 | 3
  3 | 1
)
5 (
  1 | 3
  5 | 1
)
)&");
            }
          }

          AND_WHEN("We add more nodes (6, 7)") {
            g.InsertNode(6);
            g.InsertNode(7);

            THEN("The grpah should now have 5 nodes") {
              REQUIRE(g.GetNodes().size() == 5);
              REQUIRE(g.IsNode(1));
              REQUIRE(g.IsNode(2));
              REQUIRE(g.IsNode(3));
              REQUIRE(g.IsNode(6));
              REQUIRE(g.IsNode(7));
            }
          }
        }
      }

      AND_WHEN("We remove node 2") {
        g.DeleteNode(2);
        THEN("1 and 3 are nodes, but 2 isn't") {
          REQUIRE(g.IsNode(1));
          REQUIRE(!g.IsNode(2));
          REQUIRE(g.IsNode(3));
        }
      }

      AND_WHEN("We replace node 1 to be node 4") {
        g.Replace(1, 4);

        THEN("2,3,4 are nodes") {
          REQUIRE(g.IsNode(2));
          REQUIRE(g.IsNode(3));
          REQUIRE(g.IsNode(4));
        }
      }
    }
  }
}


SCENARIO("Mergereplace will work under various conditions of graphs") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, int> g;

    WHEN("We try merge replace a non-existant node to another") {
      THEN("It will throw an exception") {
        REQUIRE_THROWS_WITH(g.MergeReplace(1, 2),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
  }

  GIVEN("A graph with one node") {
    gdwg::Graph<int, int> g{1};

    WHEN("We try merge replace an existing node into a non-existant node") {
      THEN("It will throw an exception") {
        REQUIRE_THROWS_WITH(g.MergeReplace(1, 2),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }

    WHEN("We try merge non existant node to an existant node") {
      THEN("It will throw an exception") {
        REQUIRE_THROWS_WITH(g.MergeReplace(2, 1),
          "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
      }
    }
  }

  GIVEN("A graph with 3 nodes which only have edges to itself") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);

    WHEN("we merge replace 1 into 2") {
      g.MergeReplace("1", "2");

      THEN("We expect the graph to have 1's edges merged into 2s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(2 (
  1 | 1
  2 | 2
)
3 (
  3 | 3
)
)&");
      }
    }

    WHEN("we merge replace 3 into 1") {
      g.MergeReplace("3", "1");

      THEN("We expect the graph to have 3's edges merged into 1s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(1 (
  1 | 1
  3 | 3
)
2 (
  2 | 2
)
)&");
      }
    }
  }

  GIVEN("A graph with 3 nodes and edges to itself and each other") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("1", "2", 1);
    g.InsertEdge("1", "3", 2);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);
    g.InsertEdge("2", "1", 3);
    g.InsertEdge("2", "3", 4);
    g.InsertEdge("3", "1", 5);
    g.InsertEdge("3", "2", 6);

    WHEN("we merge replace 3 into 1") {
      g.MergeReplace("3", "1");

      THEN("We expect the graph to have 3's edges merged into 1s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(1 (
  1 | 1
  2 | 1
  3 | 2
)
2 (
  1 | 3
  2 | 2
  3 | 4
)
)&");
      }
    }
  }

  GIVEN("A graph with 3 nodes and edges to itself and the next") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("1", "2", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);
    g.InsertEdge("2", "3", 4);
    g.InsertEdge("3", "1", 5);

    WHEN("we merge replace 3 into 1") {
      g.MergeReplace("3", "1");

      THEN("We expect the graph to have 3's edges merged into 1s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(1 (
  1 | 1
  2 | 1
  3 | 3
)
2 (
  2 | 2
  3 | 4
)
)&");
      }
    }
  }

  GIVEN("A graph with 3 nodes, 2 with edges to each other and a node with no edges") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "2", 1);
    g.InsertEdge("2", "2", 2);

    WHEN("we merge replace 3 into 1") {
      g.MergeReplace("3", "1");

      THEN("We expect the graph to have 3's edges merged into 1s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(1 (
  2 | 1
)
2 (
  2 | 2
)
)&");
      }
    }

    WHEN("we merge replace 1 into 3") {
      g.MergeReplace("1", "3");

      THEN("We expect the graph to have 3's edges merged into 1s (deduplicated)") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(2 (
  2 | 2
)
3 (
  2 | 1
)
)&");
      }
    }

    WHEN("we merge replace 1 into 2") {
      g.MergeReplace("1", "2");

      THEN("We expect the graph to have 1's edges merged into 2, and 3 be empty") {
        auto os = std::ostringstream();
        os << g;
        REQUIRE(os.str() == R"&(2 (
  2 | 2
)
3 (
)
)&");
      }
    }
  }
}

// Test Getters
SCENARIO("Graph information can be queried through Get Nodes/Connected/Weights") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, int> g;
    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "");

    THEN("There should be n nodes") {
      REQUIRE(g.GetNodes().size() == 0);
    }
  }

  GIVEN("A graph with one node") {
    gdwg::Graph<int, int> g{1};

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "1 (\n)\n");

    WHEN("We request the nodes") {
      auto nodes = g.GetNodes();
      THEN("There should be one node (1)") {
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == 1);
      }
    }

    WHEN("We request get connected from node 1") {
      auto nodes = g.GetConnected(1);

      THEN("There should be no nodes") {
        REQUIRE(nodes.size() == 0);
      }
    }
  }

  GIVEN("A graph with 3 nodes which only have edges to itself") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);

    WHEN("We request the nodes") {
      auto nodes = g.GetNodes();
      THEN("There should be three nodes (1,2,3,)") {
        REQUIRE(nodes.size() == 3);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "2");
        REQUIRE(nodes[2] == "3");
      }
    }

    WHEN("We request get connected from node 1") {
      auto nodes = g.GetConnected("1");

      THEN("There should be 1 nodes") {
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0] == "1");
      }
    }
  }

  GIVEN("A graph with 3 nodes and node 1 has many edges to other nodes") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);
    g.InsertEdge("1", "2", 3);
    g.InsertEdge("1", "3", 3);
    g.InsertEdge("1", "3", 2);
    g.InsertEdge("1", "3", 4);

    WHEN("We request the nodes") {
      auto nodes = g.GetNodes();
      THEN("There should be three nodes (1,2,3,)") {
        REQUIRE(nodes.size() == 3);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "2");
        REQUIRE(nodes[2] == "3");
      }
    }

    WHEN("We request get connected from node 1") {
      auto nodes = g.GetConnected("1");

      THEN("There should be 3 nodes") {
        REQUIRE(nodes.size() == 3);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "2");
        REQUIRE(nodes[2] == "3");
      }
    }

    WHEN("We request the weights from node 1 to 3") {
      auto nodes = g.GetWeights("1", "3");

      THEN("There should be 3 nodes") {
        REQUIRE(nodes.size() == 3);
        REQUIRE(nodes[0] == 2);
        REQUIRE(nodes[1] == 3);
        REQUIRE(nodes[2] == 4);
      }
    }
  }
}


SCENARIO("Graphs can be cleared") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, int> g;
    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "");
    g.Clear();
    os << g;
    REQUIRE(os.str() == "");
  }

  GIVEN("A graph with one node") {
    gdwg::Graph<int, int> g{1};

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "1 (\n)\n");
    g.Clear();
    auto os2 = std::ostringstream();
    os2 << g;
    REQUIRE(os2.str() == "");
  }

  GIVEN("A graph with 3 nodes which only have edges to itself") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == R"&(1 (
  1 | 1
)
2 (
  2 | 2
)
3 (
  3 | 3
)
)&");
    g.Clear();
    auto os2 = std::ostringstream();
    os2 << g;
    REQUIRE(os2.str() == "");
  }
}

// Test removing nodes (erase/delete)
SCENARIO("Graphs can have nodes/edges removed") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, int> g;
    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "");

    WHEN("We try to remove some node") {
      auto ret = g.DeleteNode(0);
      THEN("the delete should've returned false (since the node doesnt exist") {
        REQUIRE(ret == false);
      }
    }
  }

  GIVEN("A graph with one node") {
    gdwg::Graph<int, int> g{1};

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "1 (\n)\n");

    WHEN("We try to delete this (1)") {
      auto ret = g.DeleteNode(1);
      THEN("There should be no nodes left and ret should be true") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 0);
      }
    }

    WHEN("We try to delete an edge that doesn't exist") {
      auto ret = g.erase(1, 2, 1);

      THEN("There should be 1 node and the return should be false") {
        REQUIRE(ret == false);
        REQUIRE(g.GetNodes().size() == 1);
      }
    }
  }

  GIVEN("A graph with 3 nodes which only have edges to itself") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);

    WHEN("We delete node 1") {
      auto ret = g.DeleteNode("1");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (2,3)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "2");
        REQUIRE(nodes[1] == "3");
      }
    }

    WHEN("We delete node 2") {
      auto ret = g.DeleteNode("2");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (1,3)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "3");
      }
    }

    WHEN("We delete node 3") {
      auto ret = g.DeleteNode("3");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (1,2)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "2");
      }
    }
  }

  GIVEN("A graph with 3 nodes and node 1 has many edges to other nodes") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);
    g.InsertEdge("1", "2", 3);
    g.InsertEdge("1", "3", 3);
    g.InsertEdge("1", "3", 2);
    g.InsertEdge("1", "3", 4);

    WHEN("We delete node 1") {
      auto ret = g.DeleteNode("1");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (2,3)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "2");
        REQUIRE(nodes[1] == "3");
      }
    }

    WHEN("We delete node 2") {
      auto ret = g.DeleteNode("2");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (1,3)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "3");
      }
    }

    WHEN("We delete node 3") {
      auto ret = g.DeleteNode("3");
      auto nodes = g.GetNodes();
      THEN("There should be two nodes (1,2)") {
        REQUIRE(ret == true);
        REQUIRE(g.GetNodes().size() == 2);
        REQUIRE(nodes[0] == "1");
        REQUIRE(nodes[1] == "2");
      }
    }

    WHEN("We delete the edge 1->3 (4)") {
      auto ret = g.erase("1", "3", 4);
      THEN("This should successfully delete this edge") {
        REQUIRE(ret == true);
        auto weights = g.GetWeights("1",  "3");
        REQUIRE(weights.size() == 2);
      }

      AND_WHEN("We try delete it again") {
        auto ret = g.erase("1", "3", 4);
        THEN("The graph will not be able to delete this edge") {
          REQUIRE(ret == false);
          auto weights = g.GetWeights("1",  "3");
          REQUIRE(weights.size() == 2);
        }
      }
    }
  }
}


// Testing iterators

SCENARIO("Graphs can be cleared") {
  GIVEN("An empty graph") {
    gdwg::Graph<int, int> g;
    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "");
    g.Clear();
    os << g;
    REQUIRE(os.str() == "");
  }

  GIVEN("A graph with one node") {
    gdwg::Graph<int, int> g{1};

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == "1 (\n)\n");
    g.Clear();
    auto os2 = std::ostringstream();
    os2 << g;
    REQUIRE(os2.str() == "");
  }

  GIVEN("A graph with 3 nodes which only have edges to itself") {
    gdwg::Graph<std::string, int> g{"1", "2", "3"};

    g.InsertEdge("1", "1", 1);
    g.InsertEdge("2", "2", 2);
    g.InsertEdge("3", "3", 3);

    auto os = std::ostringstream();
    os << g;
    REQUIRE(os.str() == R"&(1 (
  1 | 1
)
2 (
  2 | 2
)
3 (
  3 | 3
)
)&");
    g.Clear();
    auto os2 = std::ostringstream();
    os2 << g;
    REQUIRE(os2.str() == "");
  }
}