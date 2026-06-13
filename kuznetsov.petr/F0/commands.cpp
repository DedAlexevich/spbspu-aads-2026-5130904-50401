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




