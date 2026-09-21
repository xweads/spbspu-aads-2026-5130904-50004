#ifndef INSTITUTE_HPP
#define INSTITUTE_HPP

#include <string>
#include <memory>
#include "BiList.hpp"

namespace dolenko
{
  class Institute
  {
  private:
    std::string name_;
    BiList< std::string > rooms_;

  public:
    explicit Institute(const std::string& name) noexcept :
      name_(name)
    {}

    const std::string& getName() const noexcept
    {
      return name_;
    }

    void assignRoom(const std::string& room)
    {
      for (auto it = rooms_.begin(); it != rooms_.end(); ++it)
      {
        if (*it == room) return;
      }
      rooms_.push_back(room);
    }

    bool hasRoom(const std::string& room) const noexcept
    {
      for (auto it = rooms_.begin(); it != rooms_.end(); ++it)
      {
        if (*it == room) return true;
      }
      return false;
    }

    BiList< std::string >::Iterator begin() noexcept
    {
      return rooms_.begin();
    }

    BiList< std::string >::Iterator end() noexcept
    {
      return rooms_.end();
    }

    BiList< std::string >::ConstIterator begin() const noexcept
    {
      return rooms_.begin();
    }

    BiList< std::string >::ConstIterator end() const noexcept
    {
      return rooms_.end();
    }

    std::size_t roomCount() const noexcept
    {
      return rooms_.size();
    }

    bool empty() const noexcept
    {
      return rooms_.empty();
    }
  };
}

#endif
