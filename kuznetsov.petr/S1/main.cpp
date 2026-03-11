#include <iostream>
#include <utility>
#include "list.hpp"


int main()
{
  namespace  kuz = kuznetsov;
  using stringListpair = std::pair< std::string, kuz::List< size_t > >;
  auto list = kuz::List< stringListpair >();
  std::string name;
  while (std::cin >> name) {
    auto currSequence = kuz::List< size_t >();
    size_t t = 0;
    while (std::cin >> t) {
      currSequence.insert(currSequence.end(), t);
    }
    stringListpair p{name, currSequence};
    list.insert(list.end(), p);
    if (std::cin.bad()) {
      return 1;
    } else if (!std::cin.eof()) {
      std::cin.clear();
    }
  }

  auto be = list.cbegin();
  do {
    std::cout << (*be).first << '\n';
    auto be2 = (*be).second.cbegin();
    do {
      std::cout << *be2 << '\n';
      be2++;
    } while (be2 != (*be).second.cbegin());
    be++;
  } while (be != list.cbegin());


}

