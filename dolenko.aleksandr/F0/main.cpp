#include <iostream>
#include <string>
#include <sstream>
#include "credentials.hpp"
#include "UniversitySystem.hpp"

namespace dolenko
{
  class CommandParser
  {
  private:
    UniversitySystem system_;

    void initializeDefaultData()
    {
      system_.addInstitute("ГИ");
      system_.addInstitute("ИСИ");
      system_.addInstitute("ИБСБ");
      system_.addInstitute("ИКНК");
      system_.addInstitute("ИММТ");
      system_.addInstitute("ИФМ");
      system_.addInstitute("ИФКСиТ");
      system_.addInstitute("ИЭТ");
      system_.addInstitute("ИЭ");
      system_.addInstitute("ФМИ");
      system_.addInstitute("ИПМЭиТ");

      system_.addRoom("101");
      system_.addRoom("102");
      system_.addRoom("201");
      system_.addRoom("202");
      system_.addRoom("301");
      system_.addRoom("302");

      system_.assignRoom("101", "ГИ");
      system_.assignRoom("102", "ИСИ");
      system_.assignRoom("201", "ИБСБ");
      system_.assignRoom("202", "ИКНК");
      system_.assignRoom("301", "ИММТ");
      system_.assignRoom("302", "ИФМ");
    }

    std::string canonicalCommand(const std::string& command) const
    {
      if (command == "add_institute" || command == "добавить_институт") return "add_institute";
      if (command == "add_room" || command == "добавить_аудиторию") return "add_room";
      if (command == "assign_room" || command == "назначить_аудиторию") return "assign_room";
      if (command == "book_room" || command == "забронировать_аудиторию") return "book_room";
      if (command == "cancel_booking" || command == "отменить_бронь") return "cancel_booking";
      if (command == "find_booking" || command == "найти_бронь") return "find_booking";
      if (command == "show_schedule" || command == "показать_расписание") return "show_schedule";
      if (command == "free_rooms" || command == "свободные_аудитории") return "free_rooms";
      if (command == "clear" || command == "очистить") return "clear";
      if (command == "help" || command == "помощь") return "help";
      if (command == "exit" || command == "выход") return "exit";
      return command;
    }

  public:
    void run()
    {
      printCredentials();
      std::cout << "\n=== Система бронирования аудиторий Политеха ===\n";
      std::cout << "Система инициализирована: 11 институтов и 6 аудиторий\n";
      std::cout << "Введите 'help' или 'помощь' для списка команд, 'exit' или 'выход' для завершения\n\n";

      initializeDefaultData();

      std::string line;
      while (std::getline(std::cin, line))
      {
        if (line.empty()) continue;

        std::string command = canonicalCommand(line.substr(0, line.find(' ')));

        if (command == "exit")
        {
          std::cout << "Goodbye!\n";
          break;
        }
        else if (command == "help")
        {
          printHelp();
        }
        else if (command == "add_institute")
        {
          handleAddInstitute(line);
        }
        else if (command == "add_room")
        {
          handleAddRoom(line);
        }
        else if (command == "assign_room")
        {
          handleAssignRoom(line);
        }
        else if (command == "book_room")
        {
          handleBookRoom(line);
        }
        else if (command == "cancel_booking")
        {
          handleCancelBooking(line);
        }
        else if (command == "find_booking")
        {
          handleFindBooking(line);
        }
        else if (command == "show_schedule")
        {
          handleShowSchedule(line);
        }
        else if (command == "free_rooms")
        {
          handleFreeRooms(line);
        }
        else if (command == "clear")
        {
          handleClear();
        }
        else
        {
          std::cout << "Unknown command: " << command << "\n";
        }
      }
    }

  private:
    void printHelp() const
    {
      std::cout << "Доступные команды:\n"
                << "  add_institute <name> / добавить_институт <name> - Добавить новый институт\n"
                << "  add_room <number> / добавить_аудиторию <number> - Добавить новую аудиторию\n"
                << "  assign_room <room_number> <institute_name> / назначить_аудиторию <room_number> <institute_name> - Назначить аудиторию институту\n"
                << "  book_room <institute> <room> <date> <time> <duration> / забронировать_аудиторию <institute> <room> <date> <time> <duration> - Забронировать аудиторию\n"
                << "  cancel_booking <booking_id> / отменить_бронь <booking_id> - Отменить бронирование\n"
                << "  find_booking <booking_id> / найти_бронь <booking_id> - Найти бронирование\n"
                << "  show_schedule <type> <name> <date> / показать_расписание <type> <name> <date> - Показать расписание\n"
                << "  free_rooms <institute> <date> <time> <duration> / свободные_аудитории <institute> <date> <time> <duration> - Свободные аудитории\n"
                << "  clear / очистить - Очистить все данные\n"
                << "  exit / выход - Завершить программу\n";
    }

    void handleAddInstitute(const std::string& line)
    {
      std::size_t pos = line.find(' ') + 1;
      std::string name = line.substr(pos);
      if (name.empty())
      {
        std::cout << "Error: Institute name required\n";
        return;
      }

      BookingResult result = system_.addInstitute(name);
      if (result.success)
      {
        std::cout << "Success: " << result.message << "\n";
      }
      else
      {
        std::cout << "Error: " << result.message << "\n";
      }
    }

    void handleAddRoom(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd, number;
      iss >> cmd >> number;
      
      if (number.empty())
      {
        std::cout << "Error: Room number required\n";
        return;
      }

      BookingResult result = system_.addRoom(number);
      if (result.success)
      {
        std::cout << "Success: " << result.message << "\n";
      }
      else
      {
        std::cout << "Error: " << result.message << "\n";
      }
    }

    void handleAssignRoom(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd, room, institute;
      iss >> cmd >> room >> institute;
      
      if (room.empty() || institute.empty())
      {
        std::cout << "Error: Both room number and institute name required\n";
        return;
      }

      BookingResult result = system_.assignRoom(room, institute);
      if (result.success)
      {
        std::cout << "Success: " << result.message << "\n";
      }
      else
      {
        std::cout << "Error: " << result.message << "\n";
      }
    }

    void handleBookRoom(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd, institute, room, date, time_start;
      std::size_t duration;

      iss >> cmd >> institute >> room >> date >> time_start >> duration;

      if (institute.empty() || room.empty() || date.empty() || time_start.empty() || duration == 0)
      {
        std::cout << "Error: All parameters required (institute room date time duration)\n";
        return;
      }

      BookingResult result = system_.bookRoom(institute, room, date, time_start, duration);
      if (result.success)
      {
        std::cout << "Success: " << result.message << " (ID: " << result.booking_id << ")\n";
      }
      else
      {
        std::cout << "Error: " << result.message << "\n";
      }
    }

    void handleCancelBooking(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd;
      std::size_t booking_id;
      iss >> cmd >> booking_id;
      
      if (booking_id == 0)
      {
        std::cout << "Error: Booking ID required\n";
        return;
      }

      BookingResult result = system_.cancelBooking(booking_id);
      if (result.success)
      {
        std::cout << "Success: " << result.message << "\n";
      }
      else
      {
        std::cout << "Error: " << result.message << "\n";
      }
    }

    void handleFindBooking(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd;
      std::size_t booking_id;
      iss >> cmd >> booking_id;
      
      if (booking_id == 0)
      {
        std::cout << "Error: Booking ID required\n";
        return;
      }

      auto booking = system_.findBooking(booking_id);
      if (booking.has_value())
      {
        const Booking& b = booking.value();
        std::cout << "Booking ID: " << b.id << "\n"
                  << "Institute: " << b.institute << "\n"
                  << "Room: " << b.room << "\n"
                  << "Date: " << b.date << "\n"
                  << "Time: " << b.time_start << "\n"
                  << "Duration: " << b.duration << " minutes\n";
      }
      else
      {
        std::cout << "Error: Booking not found\n";
      }
    }

    void handleShowSchedule(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd, type, name, date;
      iss >> cmd >> type >> name >> date;

      if (type == "аудитория" || type == "комната")
      {
        type = "room";
      }
      else if (type == "институт")
      {
        type = "institute";
      }
      
      if (type.empty() || name.empty() || date.empty())
      {
        std::cout << "Error: Type, name, and date required\n";
        return;
      }

      std::cout << "Schedule for " << type << " '" << name << "' on " << date << ":\n";

      bool found = false;
      for (auto it = system_.getSchedule().begin(); it != system_.getSchedule().end(); ++it)
      {
        const Booking& b = *it;
        if (b.date != date) continue;

        if (type == "room" && b.room == name)
        {
          std::cout << "  ID: " << b.id << " | Institute: " << b.institute
                    << " | Time: " << b.time_start << " | Duration: " << b.duration << " min\n";
          found = true;
        }
        else if (type == "institute" && b.institute == name)
        {
          std::cout << "  ID: " << b.id << " | Room: " << b.room
                    << " | Time: " << b.time_start << " | Duration: " << b.duration << " min\n";
          found = true;
        }
      }

      if (!found)
      {
        std::cout << "No bookings found\n";
      }
    }

    void handleFreeRooms(const std::string& line)
    {
      std::istringstream iss(line);
      std::string cmd, institute, date, time_start;
      std::size_t duration;

      iss >> cmd >> institute >> date >> time_start >> duration;

      if (institute.empty() || date.empty() || time_start.empty() || duration == 0)
      {
        std::cout << "Error: All parameters required (institute date time duration)\n";
        return;
      }

      const Institute* inst = system_.findInstitute(institute);
      if (!inst)
      {
        std::cout << "Error: Institute not found\n";
        return;
      }

      std::cout << "Free rooms for " << institute << " on " << date << " at " << time_start
                << " (for " << duration << " minutes):\n";

      bool found = false;
      for (auto room_it = system_.roomsBegin(); room_it != system_.roomsEnd(); ++room_it)
      {
        const Room* room = (*room_it).get();
        const std::string& room_num = room->getNumber();

        if (!inst->hasRoom(room_num)) continue;

        bool has_conflict = false;
        for (auto sched_it = system_.getSchedule().begin(); sched_it != system_.getSchedule().end(); ++sched_it)
        {
          const Booking& b = *sched_it;
          if (b.room != room_num || b.date != date) continue;

          if (b.time_start == time_start)
          {
            has_conflict = true;
            break;
          }
        }

        if (!has_conflict)
        {
          std::cout << "  " << room_num << "\n";
          found = true;
        }
      }

      if (!found)
      {
        std::cout << "No free rooms available\n";
      }
    }

    void handleClear()
    {
      system_.clear();
      std::cout << "All data cleared successfully\n";
    }
  };
}

int main()
{
  dolenko::CommandParser parser;
  parser.run();
  return 0;
}
