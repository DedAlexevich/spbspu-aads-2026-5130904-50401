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
    size_t operator()(Edge e);
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

  std::string name;
  size_t count = 0;
  while (input >> name >> count) {
    std::cout << name << '\n';
    std::cout << count << '\n';
  }

}

