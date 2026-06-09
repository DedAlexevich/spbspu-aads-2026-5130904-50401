#include "model.hpp"

bool kuznetsov::detail::StrEqual::operator()(const std::string& a, const std::string& b) const
{
  return a == b;
}



