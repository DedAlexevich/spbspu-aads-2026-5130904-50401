#ifndef OPTIMIZE_HPP
#define OPTIMIZE_HPP
#include <string>
#include <vector.hpp>
#include "model.hpp"

namespace kuznetsov {
  const size_t BRUTE_LIMIT = 9;

  void optimize(Map& map, const std::string& base);

  namespace detail {
    struct NodeIndex {
      Vector< std::string > cityNames;
      StrHashMap< size_t > cityIdx;
      Vector< std::string > typeNames;
    };

    NodeIndex buildNodeIndex(const Map& m);

    struct Segment {
      std::string a;
      std::string b;
      bool reachable;
      double cost;
      Vector< RouteStep > steps;
    };

    Segment routeSegment(const Map& m, const NodeIndex& idx, const std::string& from, const std::string& to);

    struct SegmentCache {
      Vector< Segment > segments;
      void build(const Map& m, const NodeIndex& idx, const Vector< std::string >& keys);
      const Segment& get(const std::string& a, const std::string& b) const;
    };

    Vector< std::string > keyCities(const Map& m, const std::string& base);
    double fineSum(const Map& m, const Vector< size_t >& perm);
    double sequenceCost(const Map& m, const SegmentCache& cache, const std::string& base,
      const Vector< size_t >& perm);
    void permute(const Map& m, const SegmentCache& cache, const std::string& base,
      Vector< size_t >& perm, size_t k, Vector< size_t >& best, double& bestCost);
  }
}

#endif
