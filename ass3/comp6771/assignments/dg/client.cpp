#include <iostream>
#include <string>

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

  // return 0;

  // gdwg::Graph<std::string, int> g{"hello", "how"};
  // std::cout << g.InsertEdge("hello", "how", 5) << std::endl;
  // std::cout << g.InsertEdge("hello", "how", 5) << std::endl;
  // std::cout << g.InsertEdge("how", "hello", 5) << std::endl;
  // std::cout << g << '\n';

  // return 0;

  gdwg::Graph<std::string, int> g{"A", "B", "C", "D"};
  std::cout << g.InsertEdge("A", "B", 1) << std::endl;
  std::cout << g.InsertEdge("A", "C", 2) << std::endl;
  std::cout << g.InsertEdge("A", "D", 3) << std::endl;
  std::cout << g.InsertEdge("B", "B", 1) << std::endl;
  std::cout << g << '\n';
  g.MergeReplace("A", "B");
  std::cout << g << '\n';

  return 0;

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
}

// int main() {
//   gdwg::Graph<std::string, int> g;
//   g.InsertNode("hello");
//   g.InsertNode("how");
//   g.InsertNode("are");
//   g.InsertNode("you?");

//   g.InsertEdge("hello", "how", 5);
//   g.InsertEdge("hello", "are", 8);
//   g.InsertEdge("hello", "are", 2);

//   g.InsertEdge("how", "you?", 1);
//   g.InsertEdge("how", "hello", 4);

//   g.InsertEdge("are", "you?", 3);

//   std::cout << g << '\n';

//   gdwg::Graph<std::string, int> g2{g};

//   std::cout << g2 << "\n";

//   // This is a structured binding.
//   // https://en.cppreference.com/w/cpp/language/structured_binding
//   // It allows you to unpack your tuple.
//   for (const auto& [from, to, weight] : g) {
//     std::cout << from << " -> " << to << " (weight " << weight << ")\n";
//   }
// }
