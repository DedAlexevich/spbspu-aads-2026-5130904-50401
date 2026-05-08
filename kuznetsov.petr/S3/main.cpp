#include <iostream>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/describe/class.hpp>
#include <fstream>
#include "../common/includes/top-it-vector.hpp"
#include "./HashTable.hpp"

namespace kuznetsov {

  struct Edge {
    std::string from_;
    std::string to_;
  };

  BOOST_DESCRIBE_STRUCT(Edge, (), (from_, to_));
  struct Hasher {
    size_t operator()(const Edge& e) const
    {
      boost::hash2::siphash_64 hasher(seed);
      boost::hash2::hash_append(hasher, {}, e);
      return hasher.result();
    }
    size_t operator()(const std::string& s) const
    {
      boost::hash2::siphash_64 hasher(seed);
      hasher.update(s.data(), s.size());
      return hasher.result();
    }
  private:
    size_t seed;
  };

  struct KeyComparator {
    bool operator()(const Edge& e1, const Edge& e2) const
    {
      bool f = e1.from_ == e2.from_;
      f = f && e1.to_ == e2.to_;
      return f;
    }
    bool operator()(const std::string& s1, const std::string& s2) const
    {
      return s1 == s2;
    }
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
      kuz::Edge e;
      size_t weight;
      input >> e.from_ >> e.to_ >> weight;
      if (t.has(e)) {
        t.at(e).pushBack(weight);
      } else {
        t.add(e, kuz::Vector< size_t >());
        t.at(e).pushBack(weight);
      }
    }
    graphs.add(name, t);
  }
  std::cout << graphs.getSize() << '\n';

}

