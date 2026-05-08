#include <iostream>
#include <boost/hash2/siphash.hpp>
#include <fstream>
#include "../common/includes/top-it-vector.hpp"
#include "./HashTable.hpp"

namespace kuznetsov {

  struct Edge {
    std::string from_;
    std::string to_;
  };

  struct Hasher {
    size_t operator()(const Edge& e) const;
    size_t operator()(const std::string&) const;
  };
  
  struct KeyComparator {
    bool operator()(const Edge& e) const;
    bool operator()(const std::string&) const;
  };

}

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Couldn't open file\n";
    return 1;
  }
  namespace kuz = kuznetsov;
  std::string name;
  size_t count = 0;
  using graph = kuz::HashTable< kuz::Edge, kuz::Vector< size_t >, kuz::Hasher, kuz::KeyComparator >;
  kuz::HashTable< std::string, graph, kuz::Hasher, kuz::KeyComparator > graphs;


  while (input >> name >> count) {
    graph t(count);
    for (size_t i = 0; i < count; ++i) {
      Edge e;
      size_t weight;
      input >> e.from_ >> e.to_ >> weight;
      t.add(e, weight);
    }
    graphs.add(name, t);
  }


}

