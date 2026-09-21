#ifndef UNIVERSITY_SYSTEM_HPP
#define UNIVERSITY_SYSTEM_HPP

#include <string>
#include <memory>
#include <optional>
#include "ScheduleAVL.hpp"
#include "Institute.hpp"
#include "Room.hpp"
#include "BiList.hpp"

namespace dolenko
{
  struct BookingResult
  {
    bool success;
    std::string message;
    std::size_t booking_id;

    BookingResult(bool s, const std::string& msg, std::size_t id = 0) noexcept :
      success(s),
      message(msg),
      booking_id(id)
    {}
  };

  struct TimeSlot
  {
    std::string date;
    std::string time_start;
    std::size_t duration;
  };

  class UniversitySystem
  {
  private:
    BiList< std::unique_ptr< Institute > > institutes_;
    BiList< std::unique_ptr< Room > > rooms_;
    ScheduleAVL< Booking > schedule_;
    std::size_t next_booking_id_;

    using InstListIterator = BiList< std::unique_ptr< Institute > >::Iterator;
    using InstListConstIterator = BiList< std::unique_ptr< Institute > >::ConstIterator;
    using RoomListIterator = BiList< std::unique_ptr< Room > >::Iterator;
    using RoomListConstIterator = BiList< std::unique_ptr< Room > >::ConstIterator;

    bool checkTimeConflict(
      const std::string& room,
      const std::string& date,
      const std::string& time_start,
      std::size_t duration
    ) const noexcept;

    bool timeToMinutes(const std::string& time, std::size_t& minutes) const noexcept;
    std::string minutesToTime(std::size_t minutes) const noexcept;

  public:
    UniversitySystem() noexcept :
      next_booking_id_(1)
    {}

    UniversitySystem(const UniversitySystem& other) :
      next_booking_id_(other.next_booking_id_)
    {
      for (auto it = other.institutes_.begin(); it != other.institutes_.end(); ++it)
      {
        institutes_.push_back(std::make_unique< Institute >(**it));
      }
      for (auto it = other.rooms_.begin(); it != other.rooms_.end(); ++it)
      {
        rooms_.push_back(std::make_unique< Room >(**it));
      }
      schedule_ = other.schedule_;
    }

    UniversitySystem(UniversitySystem&& other) noexcept :
      institutes_(std::move(other.institutes_)),
      rooms_(std::move(other.rooms_)),
      schedule_(std::move(other.schedule_)),
      next_booking_id_(other.next_booking_id_)
    {
      other.next_booking_id_ = 1;
    }

    UniversitySystem& operator=(const UniversitySystem& other)
    {
      if (this == &other) return *this;
      UniversitySystem tmp(other);
      swap(tmp);
      return *this;
    }

    UniversitySystem& operator=(UniversitySystem&& other) noexcept
    {
      if (this == &other) return *this;
      institutes_ = std::move(other.institutes_);
      rooms_ = std::move(other.rooms_);
      schedule_ = std::move(other.schedule_);
      next_booking_id_ = other.next_booking_id_;
      other.next_booking_id_ = 1;
      return *this;
    }

    void swap(UniversitySystem& other) noexcept
    {
      institutes_.swap(other.institutes_);
      rooms_.swap(other.rooms_);
      schedule_.swap(other.schedule_);
      std::swap(next_booking_id_, other.next_booking_id_);
    }

    BookingResult addInstitute(const std::string& name) noexcept;
    BookingResult addRoom(const std::string& number) noexcept;
    BookingResult assignRoom(const std::string& room, const std::string& institute) noexcept;
    BookingResult bookRoom(
      const std::string& institute,
      const std::string& room,
      const std::string& date,
      const std::string& time_start,
      std::size_t duration
    ) noexcept;
    BookingResult cancelBooking(std::size_t booking_id) noexcept;
    std::optional< Booking > findBooking(std::size_t booking_id) const noexcept;

    const ScheduleAVL< Booking >& getSchedule() const noexcept
    {
      return schedule_;
    }

    Institute* findInstitute(const std::string& name) noexcept;
    const Institute* findInstitute(const std::string& name) const noexcept;
    Room* findRoom(const std::string& number) noexcept;
    const Room* findRoom(const std::string& number) const noexcept;

    BiList< std::unique_ptr< Institute > >::Iterator institutesBegin() noexcept
    {
      return institutes_.begin();
    }

    BiList< std::unique_ptr< Institute > >::Iterator institutesEnd() noexcept
    {
      return institutes_.end();
    }

    BiList< std::unique_ptr< Institute > >::ConstIterator institutesBegin() const noexcept
    {
      return institutes_.begin();
    }

    BiList< std::unique_ptr< Institute > >::ConstIterator institutesEnd() const noexcept
    {
      return institutes_.end();
    }

    BiList< std::unique_ptr< Room > >::Iterator roomsBegin() noexcept
    {
      return rooms_.begin();
    }

    BiList< std::unique_ptr< Room > >::Iterator roomsEnd() noexcept
    {
      return rooms_.end();
    }

    BiList< std::unique_ptr< Room > >::ConstIterator roomsBegin() const noexcept
    {
      return rooms_.begin();
    }

    BiList< std::unique_ptr< Room > >::ConstIterator roomsEnd() const noexcept
    {
      return rooms_.end();
    }

    std::size_t instituteCount() const noexcept
    {
      return institutes_.size();
    }

    std::size_t roomCount() const noexcept
    {
      return rooms_.size();
    }

    void clear() noexcept
    {
      institutes_.clear();
      rooms_.clear();
      schedule_.clear();
      next_booking_id_ = 1;
    }
  };
}

#endif
