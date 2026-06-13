#include "commands.hpp"
#include <iostream>
#include <stdexcept>

std::string kuznetsov::detail::reqStr(std::istream& in)
{
  std::string s;
  if (!(in >> s)) {
    throw std::logic_error("Smth go bad");
  }
  return s;
}

size_t kuznetsov::detail::reqSize(std::istream& in)
{
  size_t s;
  if (!(in >> s)) {
    throw std::logic_error("Invalid argument");
  }
  return s;
}

double kuznetsov::detail::reqDouble(std::istream& in)
{
  double d;
  if (!(in >> d)) {
    throw std::logic_error("Invalid argument");
  }
  return d;
}

bool kuznetsov::detail::readOptional(std::istream& in, std::string& out)
{
  int c = in.peek();
  while (c == ' ' || c == '\t') {
    in.get();
    c = in.peek();
  }
  if (c == '\n' || c == -1) {
    return false;
  }
  return static_cast< bool >(in >> out);
}

void kuznetsov::addTransport(std::ostream&, std::istream& in, MapsController& m)
{
  m.activeMap().addTransport(detail::reqStr(in));
}

void kuznetsov::roadType(std::ostream&, std::istream& in, MapsController& m)
{
  std::string name = detail::reqStr(in);
  double k = detail::reqDouble(in);
  double b = detail::reqDouble(in);
  double n = detail::reqDouble(in);
  m.activeMap().setRoadType(name, k, b, n);
}

void kuznetsov::addCity(std::ostream&, std::istream& in, MapsController& m)
{
  m.activeMap().addCity(detail::reqStr(in));
}

void kuznetsov::addTerminal(std::ostream&, std::istream& in, MapsController& m)
{
  std::string city = detail::reqStr(in);
  std::string type = detail::reqStr(in);
  double cost = detail::reqDouble(in);
  m.activeMap().addTerminal(city, type, cost);
}





