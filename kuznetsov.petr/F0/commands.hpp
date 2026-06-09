#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "model.hpp"
#include <cstddef>
#include <iosfwd>
#include <string>

namespace kuznetsov {
  using command = void (*)(std::ostream&, std::istream&, Manager&);
  using commandConst = void (*)(std::ostream&, std::istream&, const Manager&);

  namespace detail {
    std::string reqStr(std::istream& in);
    size_t reqSize(std::istream& in);
    double reqDouble(std::istream& in);
    bool readOptional(std::istream& in, std::string& out);
  }

  void addTransport(std::ostream&, std::istream&, Manager&);
  void roadType(std::ostream&, std::istream&, Manager&);
  void addCity(std::ostream&, std::istream&, Manager&);
  void addTerminal(std::ostream&, std::istream&, Manager&);
  void addRoad(std::ostream&, std::istream&, Manager&);
  void removeCity(std::ostream&, std::istream&, Manager&);
  void removeRoad(std::ostream&, std::istream&, Manager&);
  void addOrder(std::ostream&, std::istream&, Manager&);
  void removeOrder(std::ostream&, std::istream&, Manager&);
  void optimizeCmd(std::ostream&, std::istream&, Manager&);
  void nextStep(std::ostream&, std::istream&, Manager&);
  void prevStep(std::ostream&, std::istream&, Manager&);
  void loadCmd(std::ostream&, std::istream&, Manager&);
  void newMapCmd(std::ostream&, std::istream&, Manager&);
  void switchMapCmd(std::ostream&, std::istream&, Manager&);
  void removeMapCmd(std::ostream&, std::istream&, Manager&);

  void listCities(std::ostream&, std::istream&, const Manager&);
  void listTerminals(std::ostream&, std::istream&, const Manager&);
  void listRoads(std::ostream&, std::istream&, const Manager&);
  void listOrders(std::ostream&, std::istream&, const Manager&);
  void listMaps(std::ostream&, std::istream&, const Manager&);
  void showRoute(std::ostream&, std::istream&, const Manager&);
  void saveCmd(std::ostream&, std::istream&, const Manager&);
}

#endif
