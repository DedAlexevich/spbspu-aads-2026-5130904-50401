#include <iostream>
#include <fstream>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/describe/class.hpp>
#include "../common/includes/top-it-vector.hpp"
#include "./graph.hpp"
#include "./Hasher.hpp"
#include "./HashTable.hpp"

namespace kuznetsov {
  using table = HashTable< std::string, Graph, SipHasher< std::string >, KeyComparator >;

  void sortString(Vector< std::string >&);
  void sortWeight(Vector< std::string >&);

  void graphs(std::ostream&, std::istream&, const table&);
  void vertexes(std::ostream&, std::istream&, const table&);
  void outbound(std::ostream&, std::istream&, const table&);
  void inbound(std::ostream&, std::istream&, const table&);

  void bind(std::ostream&, std::istream&, table&);
  void cut(std::ostream&, std::istream&, table&);

  void create(std::ostream&, std::istream&, table&);
  void merge(std::ostream&, std::istream&, table&);
  void extract(std::ostream&, std::istream&, table&);

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
  using command = void(*)(std::ostream&, std::istream&, kuz::table&);
  using constCommand = void(*)(std::ostream&, std::istream&, const kuz::table&);
  kuz::table grphs;
  kuz::HashTable< std::string, command, kuz::SipHasher< std::string >, kuz::KeyComparator > cmds;
  kuz::HashTable< std::string, constCommand, kuz::SipHasher< std::string >, kuz::KeyComparator > constCmds;

  while (input >> name >> count) {
    kuz::Graph t(count);
    for (size_t i = 0; i < count; ++i) {
      kuz::key e;
      size_t weight;
      input >> e.first >> e.second >> weight;
      t.addEdge(e.first, e.second, weight);
    }
    grphs.add(name, t);
  }
  std::cout << grphs.getSize() << '\n';

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.has(cmd)) {
        cmds.at(cmd)(std::cout, std::cin, grphs);
      } else {
        cmds.at(cmd)(std::cout, std::cin, grphs);
      }
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }



  }


}
