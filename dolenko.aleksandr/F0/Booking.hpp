#ifndef BOOKING_HPP
#define BOOKING_HPP

#include <string>
#include <cstddef>

namespace dolenko
{
  struct Booking
  {
    std::size_t id;
    std::string institute;
    std::string room;
    std::string date;
    std::string time_start;
    std::size_t duration;

    Booking(
      std::size_t id_,
      const std::string& inst,
      const std::string& rm,
      const std::string& d,
      const std::string& ts,
      std::size_t dur
    ) noexcept :
      id(id_),
      institute(inst),
      room(rm),
      date(d),
      time_start(ts),
      duration(dur)
    {}


    bool operator<(const Booking& other) const noexcept
    {
      if (date != other.date) return date < other.date;
      if (time_start != other.time_start) return time_start < other.time_start;
      if (room != other.room) return room < other.room;
      return institute < other.institute;
    }

    bool operator>(const Booking& other) const noexcept
    {
      return other < *this;
    }

    bool operator==(const Booking& other) const noexcept
    {
      return id == other.id;
    }

    bool operator!=(const Booking& other) const noexcept
    {
      return !(*this == other);
    }
  };
}

#endif
