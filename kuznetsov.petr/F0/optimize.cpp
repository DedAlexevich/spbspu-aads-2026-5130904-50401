#include "optimize.hpp"
#include <string>
#include <limits>
#include <stdexcept>
#include <utility>

kuznetsov::detail::NodeIndex kuznetsov::detail::buildNodeIndex(const Map& m)
{
  NodeIndex idx;
  size_t i = 0;
  for (auto it = m.cities().cbegin(); it != m.cities().cend(); ++it) {
    idx.cityNames.pushBack(it->first);
    idx.cityIdx.insert(it->first, i);
    ++i;
  }
  for (auto it = m.transports().cbegin(); it != m.transports().cend(); ++it) {
    idx.typeNames.pushBack(it->first);
  }
  return idx;
}

