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


