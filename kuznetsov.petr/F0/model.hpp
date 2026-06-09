#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector.hpp>
#include <vectorIterators.hpp>
#include "Hasher.hpp"
#include "map.hpp"
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

  struct RouteStep {
    std::string desc;
    double cost;
  };

  struct Map {
    StrHashMap< City > cities;
    map< std::string, RoadType > transports;
    Vector< Order > orders;

    Vector< RouteStep > route;
    double routeCost;
    bool hasRoute;
    size_t cursor;

    Map();

    bool hasCity(const std::string& name) const;
    bool hasTransport(const std::string& type) const;
    bool hasTerminal(const std::string& city, const std::string& type) const;
    bool hasOrder(const std::string& id) const;
    bool hasRoad(const std::string& type, const std::string& a, const std::string& b) const;

    double terminalCost(const std::string& city, const std::string& type) const;

    void addCity(const std::string& name);
    void removeCity(const std::string& name);

    void addTransport(const std::string& type);
    void setRoadType(const std::string& type, double k, double b, double n);

    void addTerminal(const std::string& city, const std::string& type, double cost);

    void addRoad(const std::string& type, const std::string& a, const std::string& b, size_t dist);
    void removeRoad(const std::string& type, const std::string& a, const std::string& b);

    void addOrder(const Order& o);
    void removeOrder(const std::string& id);

    void clear();
    void dropRoute();
  };

  struct MapController {
    map< std::string, Map > maps;
    std::string active;

    MapController();

    Map& activeMap();
    const Map& activeMap() const;

    void newMap(const std::string& name);
    void switchMap(const std::string& name);
    void removeMap(const std::string& name);
  };

}

#endif
