#ifndef HASHER_HPP
#define HASHER_HPP
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <cstddef>
#include <utility>
namespace kuznetsov {

  template< class T >
  struct SipHasher {
    SipHasher(size_t s = 0):
      seed_(s)
    {
    }

    size_t operator()(const T& p) const
    {
      boost::hash2::siphash_64 h(seed_);
      boost::hash2::hash_append(h, {}, p);
      return h.result();
    }
  private:
    size_t seed_;
  };

  template< class First, class Second >
  struct SipHasher< std::pair< First, Second > > {
    SipHasher(size_t s = 0):
      seed_(s)
    {
    }

    size_t operator()(const std::pair< First, Second >& p) const
    {
      boost::hash2::siphash_64 h(seed_);
      boost::hash2::hash_append(h, {}, p.first);
      boost::hash2::hash_append(h, {}, p.second);
      return h.result();
    }
  private:
    size_t seed_;
  };
}

#endif
