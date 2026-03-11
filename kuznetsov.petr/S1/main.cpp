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
    list.insert(list.end(), std::make_pair(name, currSequence));
    if (std::cin.bad()) {
      return 1;
    }
    std::cin.clear();
  }

  if (list.empty()) {
    std::cout << "0\n";
    return 0;
  }

  auto listBegin = list.cbegin();
  std::cout << (*listBegin).first;
  listBegin++;
  for (size_t i = 1; i < list.size(); ++i) {
    std::cout << ' ' << (*listBegin).first;
    listBegin++;
  }

  std::cout << '\n';
  size_t countRows = 0;
  listBegin = list.cbegin();
  do {
    countRows = std::max(countRows, (*listBegin).second.size());
    listBegin++;
  } while (listBegin != list.cbegin());

  if (countRows == 0) {
    std::cerr << "Empty sequences\n";
    return 1;
  }




}

