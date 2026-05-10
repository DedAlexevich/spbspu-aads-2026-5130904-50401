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


void kuznetsov::sortString(Vector< std::string >& v)
{
  sort(v);
}

void kuznetsov::sortWeight(Vector< size_t >& v)
{
  sort(v);
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
  std::cout << *it;
  ++it;
  for (; it != names.cend(); ++it) {
    out << '\n' << *it;
  }
  out << '\n';
}



