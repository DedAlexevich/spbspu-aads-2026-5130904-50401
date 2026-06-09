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
  if (!cities.contains(a)) {
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
