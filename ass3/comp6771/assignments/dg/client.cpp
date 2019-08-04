#include <iostream>
#include <string>
#include <cassert>
#include <vector>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  // gdwg::Graph<std::string, int> g;
  // {
  //   std::string s1{"Hello"};
  //   g.InsertNode(s1);
  // }

  // // Even though s1 has gone out of scope, g has its own
  // //  copied resource that it has stored, so the node
  // //  will still be in here.
  // std::cout << g.IsNode("Hello") << "\n";  // prints 'true';

  // gdwg::Graph<std::string, int> g{"hello", "how"};
  // std::cout << g.InsertEdge("hello", "how", 5) << std::endl;
  // std::cout << g.InsertEdge("hello", "how", 5) << std::endl;
  // std::cout << g.InsertEdge("how", "hello", 5) << std::endl;
  // std::cout << g << '\n';

  // gdwg::Graph<std::string, int> g{"A", "B", "C", "D"};
  // std::cout << g.InsertEdge("A", "B", 1) << std::endl;
  // std::cout << g.InsertEdge("A", "C", 2) << std::endl;
  // std::cout << g.InsertEdge("A", "D", 3) << std::endl;
  // std::cout << g.InsertEdge("B", "B", 1) << std::endl;
  // std::cout << g.InsertEdge("B", "B", 2) << std::endl;
  // std::cout << g << '\n';
  // g.MergeReplace("A", "B");
  // std::cout << g << '\n';

  std::vector<int> ok;
  std::cout << (ok.begin() == ok.end()) << std::endl;

  std::string s1{"ni hao"};
  std::string s2{"how"};
  std::string s3{"are"};
  auto e1 = std::make_tuple(s1, s2, 5.4);
  auto e2 = std::make_tuple(s2, s3, 7.6);
  auto e = std::vector<std::tuple<std::string, std::string, double>>{e1, e2};
  gdwg::Graph<std::string, double> b{e.begin(), e.end()};

  std::cout << "Created first graph" << std::endl;

  std::cout << b << std::endl;


  gdwg::Graph<std::string, int> g{"A", "B", "C"};
  g.InsertEdge("A", "B", 5);
  g.InsertEdge("C", "B", 2);
  g.InsertEdge("B", "B", 2);
  g.InsertEdge("B", "A", 2);
  g.InsertEdge("C", "A", 2);

  std::cout << g << '\n';
  const auto res = g.GetWeights("B", "A");
  for (const auto& it : res)
    std::cout << it << " ";
  std::cout << std::endl;

  std::cout << "REPLACING A -> NA" << std::endl;
  g.Replace("A", "NA");
  std::cout << "REPLACEd -> NA" << std::endl;

  std::cout << g << std::endl;


  assert(g.IsConnected("NA", "B") == 1);

  gdwg::Graph<std::string, int> g2;
  g2 = (g);
  // g2.Replace("B", "NB");


  std::cout << "-" << std::endl;
  std::cout << g << std::endl;
  std::cout << "-" << std::endl;
  g2.DeleteNode("B");
  std::cout << g2 << "\n";

  assert(g.InsertNode("B") == 0);

  for (auto it = g.begin(); it != g.end(); it++)
    std::cout << std::get<0>(*it) << std::get<1>(*it) << std::get<2>(*it)  << std::endl;
  std::cout << "\n\n";


  for (const auto& it : g.GetConnected("C")) {
    std::cout << it << " ";
  }
  std::cout << std::endl;

  auto finder = g.find("B", "NA", 2);
  std::cout << std::get<0>(*finder) << std::get<1>(*finder) << std::get<2>(*finder)  << std::endl;
  auto finder2 = g.find("D", "NA", 2);
  std::cout << (finder2 == g.end()) << std::endl;

  std::cout << "reverse\n";

  for (auto it = g.rbegin(); it != g.rend(); ++it) {
    const auto phoebe = *it;
    std::cout << std::get<0>(phoebe) << std::get<1>(phoebe) << std::get<2>(phoebe)  << std::endl;
  }
  std::cout << "\n\n reverse end\n";

  for (auto it = g2.begin(); it != g2.end(); it++) {
    std::cout << "teeehee" << std::endl;
    std::cout << std::get<0>(*it) << std::get<1>(*it) << std::get<2>(*it)  << std::endl;
  }


  std::cout << "LMAO" << std::endl;
  fflush(stdout);

  gdwg::Graph<int, int> gempty;
  std::cout << (gempty.begin() == gempty.end()) << std::endl;

  gdwg::Graph<int, int> gstartempy{1, 2};
  gstartempy.InsertEdge(2, 1, 3);
  std::cout << gstartempy << std::endl;
  for (auto it = gstartempy.rbegin(); it != gstartempy.rend(); it++) {
    std::cout << "teeehee" << std::endl;
    std::cout << std::get<0>(*it) << std::get<1>(*it) << std::get<2>(*it)  << std::endl;
  }


  std::cout << std::endl;
  std::cout << "ERASING " << std::endl;
  std::cout << g << std::endl;
  auto erit = g.begin();
  ++erit; ++erit; ++erit; ++erit;
  std::cout << std::get<0>(*erit) << std::get<1>(*erit) << std::get<2>(*erit)  << std::endl;
  auto erit2 = g.erase("B", "NA", 2);
  std::cout << "ERASED" << std::endl;
  // if (erit2 != g.cend())
  //   std::cout << std::get<0>(*erit2) << std::get<1>(*erit2) << std::get<2>(*erit2)  << std::endl;
  std::cout << erit2 << std::endl;
  std::cout << g << std::endl;

  std::vector<int> lolv{1,2,3};
  auto lolvit = lolv.begin();
  lolvit++;
  std::cout << lolvit.base() << std::endl;


  // g.InsertNode("hello");
  // g.InsertNode("how");
  // g.InsertNode("are");
  // g.InsertNode("you?");

  // g.InsertEdge("hello", "how", 5);
  // g.InsertEdge("hello", "are", 8);
  // g.InsertEdge("hello", "are", 2);

  // g.InsertEdge("how", "you?", 1);
  // g.InsertEdge("how", "hello", 4);

  // g.InsertEdge("are", "you?", 3);


  // gdwg::Graph<std::string, int> g2{g};

  // std::cout << g2 << "\n";

  // // This is a structured binding.
  // // https://en.cppreference.com/w/cpp/language/structured_binding
  // // It allows you to unpack your tuple.
  // for (const auto& [from, to, weight] : g) {
  //   std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  // }

  return 0;
}

int main2() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you?", 3);

  std::cout << g << '\n';

  gdwg::Graph<std::string, int> g2{g};

  std::cout << g2 << "\n";


  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.
  // for (const auto& [from, to, weight] : g) {
  //   std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  // }

  return 0;
}
