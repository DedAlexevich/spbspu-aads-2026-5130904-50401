#include "./graph.hpp"
#include "../common/includes/top-it-iters.hpp"

kuznetsov::Graph::Graph():
  table_(),
  vertexes_()
{}

kuznetsov::Graph::Graph(size_t c):
  table_(c),
  vertexes_()
{}

void kuznetsov::Graph::addVertexes(const std::string& name)
{
  if (vertexes_.contain(name)) {
    return;
  }
  vertexes_.pushBack(name);
}

void kuznetsov::Graph::removeVertexes(const std::string& name)
{
  if (!vertexes_.contain(name)) {
    return;
  }

  for (auto it = vertexes_.cbegin(); it != vertexes_.cend(); ++it) {
    if (*it == name) {
      vertexes_.erase(it);
      break;
    }
  }

  for (auto it = table_.begin(); it != table_.end(); ++it) {
    if ((*it).first.first == name || (*it).first.second == name) {
      table_.remove((*it).first);
    }
  }

}

void kuznetsov::Graph::addEdge(const std::string& v1, const std::string& v2, size_t w)
{
  addVertexes(v1);
  addVertexes(v2);
  auto k = std::make_pair(v1, v2);
  if (table_.has(k)) {
    table_.at(k).pushBack(w);
  } else {
    try {
      table_.add(k, Vector< size_t >());
    } catch(...) {
      table_.rehash();
      table_.add(k, Vector< size_t >());
    }
    table_.at(k).pushBack(w);
  }
}

void kuznetsov::Graph::removeEdge(const std::string& v1, const std::string& v2, size_t w)
{
  auto k = std::make_pair(v1, v2);
  if (table_.has(k)) {
    auto slot = table_.at(k);
    if (slot.getSize() > 1) {
      for (auto it = slot.cbegin(); it != slot.cend(); ++it) {
        if ((*it) == w) {
          slot.erase(it);
          return;
        }
      }
    } else {
      table_.remove(k);
    }
  }
}


