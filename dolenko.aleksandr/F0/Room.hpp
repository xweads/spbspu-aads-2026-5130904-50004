#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>
#include <memory>
#include "BiList.hpp"

namespace dolenko
{
  class Room
  {
  private:
    std::string number_;
    BiList< std::string > institutes_;

  public:
    explicit Room(const std::string& number) noexcept :
      number_(number)
    {}

    const std::string& getNumber() const noexcept
    {
      return number_;
    }

    void assignInstitute(const std::string& institute)
    {
      for (auto it = institutes_.begin(); it != institutes_.end(); ++it)
      {
        if (*it == institute) return;
      }
      institutes_.push_back(institute);
    }

    bool hasInstitute(const std::string& institute) const noexcept
    {
      for (auto it = institutes_.begin(); it != institutes_.end(); ++it)
      {
        if (*it == institute) return true;
      }
      return false;
    }

    BiList< std::string >::Iterator begin() noexcept
    {
      return institutes_.begin();
    }

    BiList< std::string >::Iterator end() noexcept
    {
      return institutes_.end();
    }

    BiList< std::string >::ConstIterator begin() const noexcept
    {
      return institutes_.begin();
    }

    BiList< std::string >::ConstIterator end() const noexcept
    {
      return institutes_.end();
    }

    std::size_t instituteCount() const noexcept
    {
      return institutes_.size();
    }

    bool empty() const noexcept
    {
      return institutes_.empty();
    }
  };
}

#endif
