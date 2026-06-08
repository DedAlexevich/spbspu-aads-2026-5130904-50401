#ifndef MATH_FUNCS_HPP
#define MATH_FUNCS_HPP
#include <cstddef>
#include <limits>
#include <queue.hpp>
#include <stack.hpp>
#include <string>

namespace kuznetsov {

  namespace detail {
    bool isOperation(const std::string& c);
    size_t getPriority(const std::string& c);
  }
  using lli_t = long long;
  const lli_t MAX = std::numeric_limits< lli_t >::max();
  const lli_t MIN = std::numeric_limits< lli_t >::min();

  Queue< std::string > infixToPostfix(Queue< std::string > infix);

  lli_t add(const lli_t& a, const lli_t& b);
  lli_t sub(const lli_t& a, const lli_t& b);
  lli_t mul(const lli_t& a, const lli_t& b);
  lli_t div(const lli_t& a, const lli_t& b);
  lli_t mod(const lli_t& a, const lli_t& b);
  lli_t bitShiftToRight(const lli_t& a, const lli_t& b);

  using stackOfInfixExpression = Stack< Queue< std::string > >;
  stackOfInfixExpression getExpressions(std::istream& in);
  lli_t calculatePostfix(Queue< std::string > postfix);
  Queue< lli_t > calculateStackOfInfix(stackOfInfixExpression infix);
}

#endif
