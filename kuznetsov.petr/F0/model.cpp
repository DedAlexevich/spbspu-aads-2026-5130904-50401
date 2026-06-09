#include "model.hpp"
#include <math.h>
bool kuznetsov::detail::StrEqual::operator()(const std::string& a, const std::string& b) const
{
  return a == b;
}

double kuznetsov::RoadType::cost(double x) const
{
  return k * std::pow(x, n) + b;
}

kuznetsov::Map::Map():
  cities(),
  transports(),
  orders(),
  route(),
  routeCost(0.0),
  hasRoute(false),
  cursor(0)
{}

bool kuznetsov::Map::hasCity(const std::string& name) const
{
  return cities.contains(name);
}

bool kuznetsov::Map::hasTransport(const std::string& type) const
{
  return transports.contains(type);
}

bool kuznetsov::Map::hasTerminal(const std::string& city, const std::string& type) const
{
  return cities.contains(city) && cities.at(city).terminals.contains(type);
}

bool kuznetsov::Map::hasOrder(const std::string& id) const
{
  return orders.contain(id);
}

bool kuznetsov::Map::hasRoad(const std::string& type, const std::string& a, const std::string& b) const
{
  if (!hasCity(a)) {
    return false;
  }
  const City& ca = cities.at(a);
  if (!ca.roads.contains(type)) {
    return false;
  }
  for (size_t i = 0; i < ca.roads.getSize(); ++i) {
    if (ca.roads[i].to == b) {
      return true;
    }
  }
  return false;
}

double kuznetsov::Map::terminalCost(const std::string& city, const std::string& type) const
{
  return cities.at(city).terminals.at(type);
}

void kuznetsov::Map::addCity(const std::string& name)
{
  if (hasCity(name)) {
    throw std::logic_error("This city already exist");
  }
  cities.insert(name, City());
  dropRoute();
}

bool kuznetsov::detail::edgeListRemove(Vector< Edge >& edges, const std::string& to)
{
  for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
    if (it->to == to) {
      edges.erase(it);
      return true;
    }
  }
  return false;
}

void kuznetsov::Map::removeCity(const std::string& name)
{
  if (!hasCity(name)) {
    throw std::logic_error("This city doesnt exist");
  }
  Vector< Order > kept;
  for (auto it = orders.begin(); it != orders.end(); ++it) {
    if (it->from != name && it->to != name) {
      kept.pushBack(*it);
    }
  }
  cities.erase(name);
  for (auto it = cities.begin(); it != cities.end(); ++it) {
    City& c = it->second;
    for (auto rt = c.roads.begin(); rt != c.roads.end(); ++rt) {
      detail::edgeListRemove(rt->second, name);
    }
  }
  orders = kept;
  dropRoute();
}

void kuznetsov::Map::addTransport(const std::string& type)
{
  if (hasTransport(type)) {
    throw std::logic_error("Such transport alredy exist");
  }
  transports.insert(type, RoadType{ 1, 0, 1 });
}

void kuznetsov::Map::setRoadType(const std::string& type, double k, double b, double n)
{
  RoadType r{ k, b, n };
  transports.insert(type, r);
  dropRoute();
}

void kuznetsov::Map::addTerminal(const std::string& city, const std::string& type, double cost)
{
  if (!hasCity(city)) {
    throw std::logic_error("This city doesnt exist");
  }
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  cities.at(city).terminals.insert(type, cost);
  dropRoute();
}

void kuznetsov::Map::addRoad(const std::string& type, const std::string& a, const std::string& b, size_t dist)
{
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  if (!hasCity(a) || !hasCity(b)) {
    throw std::logic_error("No such city");
  }
  if (a == b) {
    throw std::logic_error("Road must connect two different cities");
  }
  if (hasRoad(type, a, b)) {
    throw std::logic_error("Road already exists");
  }

  City& ca = cities.at(a);
  if (!ca.roads.contains(type)) {
    ca.roads.insert(type, Vector< Edge >());
  }
  Edge ea{ b, dist };
  ca.roads.at(type).pushBack(ea);

  City& cb = cities.at(b);
  if (!cb.roads.contains(type)) {
    cb.roads.insert(type, Vector< Edge >());
  }
  Edge eb{ a, dist };
  cb.roads.at(type).pushBack(eb);

  dropRoute();
}

void kuznetsov::Map::removeRoad(const std::string& type, const std::string& a, const std::string& b)
{
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  if (!hasCity(a) || !hasCity(b)) {
    throw std::logic_error("No such city");
  }
  if (a == b) {
    throw std::logic_error("Road must connect two different cities");
  }
  if (!hasRoad(type, a, b)) {
    throw std::logic_error("Road doesnt exists");
  }
  detail::edgeListRemove(cities.at(a).roads.at(type), b);
  detail::edgeListRemove(cities.at(b).roads.at(type), a);
  dropRoute();
}

void kuznetsov::Map::addOrder(const Order& o)
{
  if (hasOrder(o.id)) {
    throw std::logic_error("Order already exist");
  }
  if (!hasCity(o.from) || !hasCity(o.to)) {
    throw std::logic_error("No such city");
  }
  if (o.importance < 1 || o.importance > 10) {
    throw std::logic_error("Importance must be in [1, 10]");
  }
  orders.pushBack(o);
  dropRoute();
}

void kuznetsov::Map::removeOrder(const std::string& id)
{
  for (auto it = orders.cbegin(); it != orders.cend(); ++it) {
    if (it->id == id) {
      orders.erase(it);
      dropRoute();
      return;
    }
  }
  throw std::logic_error("No such order");
}

void kuznetsov::Map::clear()
{
  cities = StrHashMap< City >();
  transports = map< std::string, RoadType >();
  orders = Vector< Order >;
  dropRoute();
}

void kuznetsov::Map::dropRoute()
{
  route = Vector< RouteStep >();
  routeCost = 0.0;
  hasRoute = false;
  cursor = 0;
}
