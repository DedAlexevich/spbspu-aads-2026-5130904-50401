#include "./commands.hpp"
#include <iostream>

template< class T >
void sort(kuznetsov::Vector< T >& v) {
  for (size_t i = 0; i < v.getSize(); ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (v[j] < v[min]) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[i], v[min]);
    }
  }
}

void kuznetsov::graphs(std::ostream& out, std::istream&, const table& t)
{
  if (t.getSize() == 0) {
    out << '\n';
    return;
  }
  Vector< std::string > names;
  for (auto it = t.cbegin(); it != t.cend(); ++it) {
    names.pushBack((*it).first);
  }
  sort(names);
  auto it = names.cbegin();
  for (; it != names.cend(); ++it) {
    out << *it << '\n';
  }
}

void kuznetsov::vertexes(std::ostream& out, std::istream& in, const table& t)
{
  std::string name;
  in >> name;
  if (!t.has(name)) {
    throw std::logic_error("Graph doesnt exist");
  }

  const Graph& g = t.at(name);
  if (g.vertexes_.isEmpty()) {
    out << '\n';
    return;
  }
  Vector< std::string > vrts(g.vertexes_);
  sort(vrts);
  for (auto it = vrts.cbegin(); it != vrts.cend(); ++it) {
    out << *it << '\n';
  }
}


