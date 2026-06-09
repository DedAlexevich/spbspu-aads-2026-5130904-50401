#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector.hpp>
#include <vectorIterators.hpp>
#include "Hasher.hpp"
#include "unorderedMap.hpp"

namespace kuznetsov {
  namespace detail {
    struct StrEqual {
      bool operator()(const std::string& a, const std::string& b) const;
    };
  }

  struct RoadType {
    double k;
    double b;
    double n;
    double cost(double x) const;
  };

  template< class V >
  using StrHashMap = unorderedMap< std::string, V, SipHasher< std::string >, detail::StrEqual >;

  struct Edge {
    std::string to;
    size_t dist;
  };

  struct City {
    StrHashMap< Vector< Edge > > roads;
    StrHashMap< double > terminals;
  };

  struct Order {
    std::string id;
    std::string from;
    std::string to;
    size_t importance;
  };
}

#endif
