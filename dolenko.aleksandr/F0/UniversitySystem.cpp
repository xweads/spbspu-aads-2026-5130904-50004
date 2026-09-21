#include "UniversitySystem.hpp"
#include <sstream>
#include <iostream>

namespace dolenko
{
  bool UniversitySystem::timeToMinutes(const std::string& time, std::size_t& minutes) const noexcept
  {
    if (time.length() != 5 || time[2] != ':')
    {
      return false;
    }
    try
    {
      int hours = std::stoi(time.substr(0, 2));
      int mins = std::stoi(time.substr(3, 2));
      if (hours < 0 || hours > 23 || mins < 0 || mins > 59)
      {
        return false;
      }
      minutes = hours * 60 + mins;
      return true;
    }
    catch (...)
    {
      return false;
    }
  }

  std::string UniversitySystem::minutesToTime(std::size_t minutes) const noexcept
  {
    std::size_t hours = minutes / 60;
    std::size_t mins = minutes % 60;
    char buffer[6];
    snprintf(buffer, sizeof(buffer), "%02zu:%02zu", hours, mins);
    return std::string(buffer);
  }

  bool UniversitySystem::checkTimeConflict(
    const std::string& room,
    const std::string& date,
    const std::string& time_start,
    std::size_t duration
  ) const noexcept
  {
    std::size_t start_minutes;
    if (!timeToMinutes(time_start, start_minutes))
    {
      return true;
    }
    std::size_t end_minutes = start_minutes + duration;

    for (auto it = schedule_.begin(); it != schedule_.end(); ++it)
    {
      const Booking& booking = *it;
      if (booking.room != room || booking.date != date)
      {
        continue;
      }

      std::size_t booking_start;
      if (!timeToMinutes(booking.time_start, booking_start))
      {
        continue;
      }
      std::size_t booking_end = booking_start + booking.duration;

      if (!(end_minutes <= booking_start || start_minutes >= booking_end))
      {
        return true;
      }
    }
    return false;
  }

  BookingResult UniversitySystem::addInstitute(const std::string& name) noexcept
  {
    if (name.empty())
    {
      return BookingResult(false, "Institute name cannot be empty", 0);
    }

    if (findInstitute(name) != nullptr)
    {
      return BookingResult(false, "Institute already exists", 0);
    }

    institutes_.push_back(std::make_unique< Institute >(name));
    return BookingResult(true, "Institute added successfully", 0);
  }

  BookingResult UniversitySystem::addRoom(const std::string& number) noexcept
  {
    if (number.empty())
    {
      return BookingResult(false, "Room number cannot be empty", 0);
    }

    if (findRoom(number) != nullptr)
    {
      return BookingResult(false, "Room already exists", 0);
    }

    rooms_.push_back(std::make_unique< Room >(number));
    return BookingResult(true, "Room added successfully", 0);
  }

  BookingResult UniversitySystem::assignRoom(const std::string& room, const std::string& institute) noexcept
  {
    Institute* inst = findInstitute(institute);
    if (!inst)
    {
      return BookingResult(false, "Institute not found", 0);
    }

    Room* rm = findRoom(room);
    if (!rm)
    {
      return BookingResult(false, "Room not found", 0);
    }

    inst->assignRoom(room);
    rm->assignInstitute(institute);
    return BookingResult(true, "Room assigned successfully", 0);
  }

  BookingResult UniversitySystem::bookRoom(
    const std::string& institute,
    const std::string& room,
    const std::string& date,
    const std::string& time_start,
    std::size_t duration
  ) noexcept
  {
    Institute* inst = findInstitute(institute);
    if (!inst)
    {
      return BookingResult(false, "Institute not found", 0);
    }

    if (!inst->hasRoom(room))
    {
      return BookingResult(false, "Room not assigned to this institute", 0);
    }

    if (duration == 0)
    {
      return BookingResult(false, "Duration must be greater than 0", 0);
    }

    if (checkTimeConflict(room, date, time_start, duration))
    {
      return BookingResult(false, "Time slot is occupied", 0);
    }

    Booking booking(next_booking_id_, institute, room, date, time_start, duration);

    ScheduleAVL< Booking > backup = schedule_;
    if (schedule_.insert(booking))
    {
      std::size_t id = next_booking_id_;
      ++next_booking_id_;
      return BookingResult(true, "Booking created successfully", id);
    }
    else
    {
      schedule_ = backup;
      return BookingResult(false, "Failed to create booking", 0);
    }
  }

  BookingResult UniversitySystem::cancelBooking(std::size_t booking_id) noexcept
  {
    auto opt = findBooking(booking_id);
    if (!opt.has_value())
    {
      return BookingResult(false, "Booking not found", 0);
    }

    Booking booking = opt.value();
    if (schedule_.remove(booking))
    {
      return BookingResult(true, "Booking cancelled successfully", booking_id);
    }
    else
    {
      return BookingResult(false, "Failed to cancel booking", 0);
    }
  }

  std::optional< Booking > UniversitySystem::findBooking(std::size_t booking_id) const noexcept
  {
    for (auto it = schedule_.begin(); it != schedule_.end(); ++it)
    {
      if ((*it).id == booking_id)
      {
        return *it;
      }
    }
    return std::nullopt;
  }

  Institute* UniversitySystem::findInstitute(const std::string& name) noexcept
  {
    for (auto it = institutes_.begin(); it != institutes_.end(); ++it)
    {
      if ((*it)->getName() == name)
      {
        return (*it).get();
      }
    }
    return nullptr;
  }

  const Institute* UniversitySystem::findInstitute(const std::string& name) const noexcept
  {
    for (auto it = institutes_.begin(); it != institutes_.end(); ++it)
    {
      if ((*it)->getName() == name)
      {
        return (*it).get();
      }
    }
    return nullptr;
  }

  Room* UniversitySystem::findRoom(const std::string& number) noexcept
  {
    for (auto it = rooms_.begin(); it != rooms_.end(); ++it)
    {
      if ((*it)->getNumber() == number)
      {
        return (*it).get();
      }
    }
    return nullptr;
  }

  const Room* UniversitySystem::findRoom(const std::string& number) const noexcept
  {
    for (auto it = rooms_.begin(); it != rooms_.end(); ++it)
    {
      if ((*it)->getNumber() == number)
      {
        return (*it).get();
      }
    }
    return nullptr;
  }
}
