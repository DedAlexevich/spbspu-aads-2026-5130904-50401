#include "./commands.hpp"
#include <iostream>
#include <utility>
#include "../common/includes/top-it-vector.hpp"

struct PairComparator {
  using pair_t = std::pair< std::string, size_t >;
  bool operator()(const pair_t& p1, const pair_t& p2)
  {
    if (p1.first != p2.first) {
      return p1.first < p2.first;
    }
    return p1.second < p2.second;
  }
};

template< class T >
struct Comparator {
  bool operator()(const T& p1, const T& p2)
  {
    return p1 < p2;
  }
};

template< class T, class Cmp >
void sort(kuznetsov::Vector< T >& v, Cmp cmp) {
  for (size_t i = 0; i < v.getSize(); ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (cmp(v[j],v[min])) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[i], v[min]);
    }
  }
}

void kuznetsov::graphs(std::ostream& out, std::istream&, const table& t)
{
  if (t.getSize() == 0) {
    out << '\n';
    return;
  }
  Vector< std::string > names;
  for (auto it = t.cbegin(); it != t.cend(); ++it) {
    names.pushBack((*it).first);
  }
  sort(names, Comparator< std::string >{});
  auto it = names.cbegin();
  for (; it != names.cend(); ++it) {
    out << *it << '\n';
  }
}

void kuznetsov::vertexes(std::ostream& out, std::istream& in, const table& t)
{
  std::string name;
  in >> name;
  if (!t.has(name)) {
    throw std::logic_error("Graph doesnt exist");
  }

  const Graph& g = t.at(name);
  if (g.vertexes_.isEmpty()) {
    out << '\n';
    return;
  }
  Vector< std::string > vrts(g.vertexes_);
  sort(vrts, Comparator< std::string >{});
  for (auto it = vrts.cbegin(); it != vrts.cend(); ++it) {
    out << *it << '\n';
  }
}

void kuznetsov::outbound(std::ostream& out, std::istream& in, const table& t)
{
  std::string nameGraph;
  in >> nameGraph;

  if (!t.has(nameGraph)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const Graph& g = t.at(nameGraph);
  std::string v;
  in >> v;
  if (!g.vertexes_.contain(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }
  Vector< std::pair< std::string, size_t > > res;
  auto it = g.table_.begin();
  while (it != g.table_.end()) {
    if ((*it).first.first == v) {
      const Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        res.pushBack(std::make_pair((*it).first.second, weights[i]));
      }
    }
    ++it;
  }
  sort(res, PairComparator{});
  for (auto it = res.cbegin(); it != res.cend(); ++it) {
    out << (*it).first << ' ' << (*it).second << '\n';
  }

}

void kuznetsov::inbound(std::ostream& out, std::istream& in, const table& t)
{
  std::string nameGraph;
  in >> nameGraph;

  if (!t.has(nameGraph)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  const Graph& g = t.at(nameGraph);
  std::string v;
  in >> v;
  if (!g.vertexes_.contain(v)) {
    throw std::logic_error("Such vertex doesnt exist");
  }
  Vector< std::pair< std::string, size_t > > res;
  auto it = g.table_.begin();
  while (it != g.table_.end()) {
    if ((*it).first.second == v) {
      const Vector< size_t >& weights = (*it).second;
      for (size_t i = 0; i < weights.getSize(); ++i) {
        res.pushBack(std::make_pair((*it).first.first, weights[i]));
      }
    }
    ++it;
  }
  sort(res, PairComparator{});
  for (auto it = res.cbegin(); it != res.cend(); ++it) {
    out << (*it).first << ' ' << (*it).second << '\n';
  }

}

void kuznetsov::bind(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (!t.has(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  t.at(name).addEdge(v1, v2, w);
}

void kuznetsov::cut(std::ostream&, std::istream& in, table& t)
{
  std::string name;
  in >> name;
  if (!t.has(name)) {
    throw std::logic_error("Such graph doesnt exist");
  }
  std::string v1, v2;
  size_t w;
  in >> v1 >> v2 >> w;
  Graph& g = t.at(name);
  bool f = !g.vertexes_.contain(v1);
  f = f || !g.vertexes_.contain(v2);
  f = f || !g.table_.has(std::make_pair(v1, v2));
  f = f || !g.table_.at(std::make_pair(v1, v2)).contain(w);
  if (f) {
    throw std::logic_error("Vertex or edge with this weight doesnt exist");
  }
  g.removeEdge(v1, v2, w);
}

