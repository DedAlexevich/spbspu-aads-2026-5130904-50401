#include "optimize.hpp"
#include <limits>
#include <stdexcept>
#include <string>
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

kuznetsov::detail::Segment kuznetsov::detail::routeSegment(const Map& map, const NodeIndex& idx,
  const std::string& from, const std::string& to)
{
  constexpr double INF = std::numeric_limits< double >::infinity();
  constexpr size_t NPOS = std::numeric_limits< double >::max();

  Segment result{ from, to, false, 0.0, Vector< RouteStep >() };
  if (from == to) {
    result.reachable = true;
    return result;
  }

  size_t cityCount = idx.cityNames.getSize();
  size_t typeCount = idx.typeNames.getSize();
  if (typeCount == 0 || !idx.cityIdx.contains(from) || !idx.cityIdx.contains(to)) {
    return result;
  }
  size_t fromIdx = idx.cityIdx.at(from);
  size_t toIdx = idx.cityIdx.at(to);
  size_t nodeCount = cityCount * typeCount;

  NodeState init{ INF, false, NPOS, EdgeKind::Board, 0, 0.0, 0 };
  Vector< NodeState > state(nodeCount, init);

  for (size_t t = 0; t < typeCount; ++t) {
    size_t node = fromIdx * typeCount + t;
    state[node].dist = 0.0;
    state[node].prev = NPOS;
    state[node].kind = EdgeKind::Board;
    state[node].type = t;
  }

  for (size_t iter = 0; iter < nodeCount; ++iter) {
    size_t curNode = NPOS;
    double best = INF;
    for (size_t i = 0; i < nodeCount; ++i) {
      if (!state[i].done && state[i].dist < best) {
        best = state[i].dist;
        curNode = i;
      }
    }
    if (curNode == NPOS) {
      break;
    }
    state[curNode].done = true;
    size_t curCity = curNode / typeCount;
    size_t curType = curNode % typeCount;
    const std::string& cityName = idx.cityNames[curCity];
    const std::string& typeName = idx.typeNames[curType];
    const City& city = map.cities().at(cityName);

    if (city.roads.contains(typeName)) {
      const RoadType& price = map.transports().at(typeName);
      const Vector< Edge >& roads = city.roads.at(typeName);
      for (size_t e = 0; e < roads.getSize(); ++e) {
        size_t nextNode = idx.cityIdx.at(roads[e].to) * typeCount + curType;
        double weight = price.cost(static_cast< double >(roads[e].dist));
        if (state[curNode].dist + weight < state[nextNode].dist) {
          state[nextNode].dist = state[curNode].dist + weight;
          state[nextNode].prev = curNode;
          state[nextNode].kind = EdgeKind::Road;
          state[nextNode].len = roads[e].dist;
          state[nextNode].cost = weight;
          state[nextNode].type = curType;
        }
      }
    }

    if (city.terminals.contains(typeName)) {
      for (size_t t = 0; t < typeCount; ++t) {
        if (t == curType || !city.terminals.contains(idx.typeNames[t])) {
          continue;
        }
        size_t nextNode = curCity * typeCount + t;
        double weight = city.terminals.at(idx.typeNames[t]);
        if (state[curNode].dist + weight < state[nextNode].dist) {
          state[nextNode].dist = state[curNode].dist + weight;
          state[nextNode].prev = curNode;
          state[nextNode].kind = EdgeKind::Transship;
          state[nextNode].len = 0;
          state[nextNode].cost = weight;
          state[nextNode].type = t;
        }
      }
    }
  }

  size_t endNode = NPOS;
  double endCost = INF;
  for (size_t t = 0; t < typeCount; ++t) {
    size_t node = toIdx * typeCount + t;
    if (state[node].dist < endCost) {
      endCost = state[node].dist;
      endNode = node;
    }
  }
  if (endNode == NPOS) {
    return result;
  }

  Vector< size_t > chain;
  for (size_t node = endNode; node != NPOS; node = state[node].prev) {
    chain.pushBack(node);
  }
  for (size_t k = chain.getSize(); k > 0; --k) {
    size_t node = chain[k - 1];
    const std::string& city = idx.cityNames[node / typeCount];
    const std::string& typeName = idx.typeNames[state[node].type];
    if (state[node].kind == EdgeKind::Transship) {
      RouteStep step{ "  switch->" + typeName + " @" + city + " fee ", state[node].cost, true };
      result.steps.pushBack(step);
    } else if (state[node].kind == EdgeKind::Road) {
      const std::string& prevCity = idx.cityNames[state[node].prev / typeCount];
      RouteStep step{ "  " + typeName + " " + prevCity + "->" + city + " (" + std::to_string(state[node].len)
                          + ") cost ",
                      state[node].cost, true };
      result.steps.pushBack(step);
    }
  }
  result.reachable = true;
  result.cost = endCost;
  return result;
}

void kuznetsov::detail::SegmentCache::build(const Map& map, const NodeIndex& idx, const Vector< std::string >& keys)
{
  for (size_t i = 0; i < keys.getSize(); ++i) {
    for (size_t j = 0; j < keys.getSize(); ++j) {
      segments.pushBack(routeSegment(map, idx, keys[i], keys[j]));
    }
  }
}

const kuznetsov::detail::Segment& kuznetsov::detail::SegmentCache::get(const std::string& from,
  const std::string& to) const
{
  for (size_t i = 0; i < segments.getSize(); ++i) {
    if (segments[i].from == from && segments[i].to == to) {
      return segments[i];
    }
  }
  throw std::logic_error("Segment not precomputed");
}

kuznetsov::Vector< std::string > kuznetsov::detail::keyCities(const Map& map, const std::string& base)
{
  Vector< std::string > keys;
  keys.pushBack(base);
  for (size_t i = 0; i < map.orders().getSize(); ++i) {
    if (!keys.contain(map.orders()[i].from)) {
      keys.pushBack(map.orders()[i].from);
    }
    if (!keys.contain(map.orders()[i].to)) {
      keys.pushBack(map.orders()[i].to);
    }
  }
  return keys;
}
