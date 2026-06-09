#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>

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
}

#endif
