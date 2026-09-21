#ifndef AVL_NODE_HPP
#define AVL_NODE_HPP

#include <memory>
#include "Booking.hpp"

namespace dolenko
{
  template< class T >
  struct AVLNode
  {
    T data;
    std::unique_ptr< AVLNode< T > > left;
    std::unique_ptr< AVLNode< T > > right;
    int height;

    explicit AVLNode(const T& val) noexcept :
      data(val),
      left(nullptr),
      right(nullptr),
      height(1)
    {}

    explicit AVLNode(T&& val) noexcept :
      data(std::move(val)),
      left(nullptr),
      right(nullptr),
      height(1)
    {}
  };
}

#endif
