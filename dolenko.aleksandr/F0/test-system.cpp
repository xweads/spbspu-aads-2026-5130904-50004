#include <iostream>
#include <cassert>
#include "UniversitySystem.hpp"

using namespace dolenko;

void testAddInstitute()
{
  UniversitySystem system;

  BookingResult result = system.addInstitute("MIT");
  assert(result.success);

  // Adding duplicate should fail
  result = system.addInstitute("MIT");
  assert(!result.success);

  // Empty name should fail
  result = system.addInstitute("");
  assert(!result.success);
  std::cout << "✓ Add Institute\n";
}

void testAddRoom()
{
  UniversitySystem system;

  BookingResult result = system.addRoom("101");
  assert(result.success);

  // Adding duplicate should fail
  result = system.addRoom("101");
  assert(!result.success);
  std::cout << "✓ Add Room\n";
}

void testAssignRoom()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addRoom("101");

  BookingResult result = system.assignRoom("101", "MIT");
  assert(result.success);

  // Assign non-existent room
  result = system.assignRoom("999", "MIT");
  assert(!result.success);

  // Assign to non-existent institute
  result = system.assignRoom("101", "FAKE");
  assert(!result.success);
  std::cout << "✓ Assign Room to Institute\n";
}

void testBookRoom()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addRoom("101");
  system.assignRoom("101", "MIT");

  BookingResult result = system.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);
  assert(result.success);
  assert(result.booking_id > 0);

  // Check booking exists
  auto booking = system.findBooking(result.booking_id);
  assert(booking.has_value());
  assert(booking.value().institute == "MIT");
  assert(booking.value().room == "101");
  std::cout << "✓ Book Room\n";
}

void testBookingTimeConflict()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addInstitute("Harvard");
  system.addRoom("101");
  system.assignRoom("101", "MIT");
  system.assignRoom("101", "Harvard");

  BookingResult result1 = system.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);
  assert(result1.success);

  // Same time, same room, different institute - should fail (global check)
  BookingResult result2 = system.bookRoom("Harvard", "101", "2025-01-15", "09:00", 60);
  assert(!result2.success);

  // Non-overlapping time - should succeed
  BookingResult result3 = system.bookRoom("Harvard", "101", "2025-01-15", "10:30", 60);
  assert(result3.success);
  std::cout << "✓ Booking Time Conflict Detection\n";
}

void testCancelBooking()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addRoom("101");
  system.assignRoom("101", "MIT");

  BookingResult result = system.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);
  std::size_t booking_id = result.booking_id;

  assert(system.findBooking(booking_id).has_value());

  result = system.cancelBooking(booking_id);
  assert(result.success);
  assert(!system.findBooking(booking_id).has_value());
  std::cout << "✓ Cancel Booking\n";
}

void testFindNonExistentBooking()
{
  UniversitySystem system;

  auto booking = system.findBooking(999);
  assert(!booking.has_value());
  std::cout << "✓ Find Non-Existent Booking\n";
}

void testClearSystem()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addRoom("101");
  system.assignRoom("101", "MIT");
  system.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);

  system.clear();

  assert(system.instituteCount() == 0);
  assert(system.roomCount() == 0);
  assert(system.getSchedule().size() == 0);
  std::cout << "✓ Clear System\n";
}

void testCopySemantics()
{
  UniversitySystem system1;

  system1.addInstitute("MIT");
  system1.addRoom("101");
  system1.assignRoom("101", "MIT");
  system1.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);

  UniversitySystem system2 = system1;

  assert(system2.instituteCount() == 1);
  assert(system2.roomCount() == 1);
  assert(system2.getSchedule().size() == 1);

  // Verify independence
  system2.clear();
  assert(system1.instituteCount() == 1);
  assert(system2.instituteCount() == 0);
  std::cout << "✓ Copy-and-Swap Semantics\n";
}

void testZeroDurationBooking()
{
  UniversitySystem system;

  system.addInstitute("MIT");
  system.addRoom("101");
  system.assignRoom("101", "MIT");

  BookingResult result = system.bookRoom("MIT", "101", "2025-01-15", "09:00", 0);
  assert(!result.success);
  std::cout << "✓ Zero Duration Booking Validation\n";
}

void testMultipleInstitutesAndRooms()
{
  UniversitySystem system;

  // Add institutes
  system.addInstitute("MIT");
  system.addInstitute("Harvard");
  system.addInstitute("Stanford");

  // Add rooms
  system.addRoom("101");
  system.addRoom("102");
  system.addRoom("103");

  // Assign rooms to institutes
  system.assignRoom("101", "MIT");
  system.assignRoom("101", "Harvard");
  system.assignRoom("102", "Harvard");
  system.assignRoom("102", "Stanford");
  system.assignRoom("103", "MIT");
  system.assignRoom("103", "Stanford");

  // Create bookings
  BookingResult r1 = system.bookRoom("MIT", "101", "2025-01-15", "09:00", 60);
  BookingResult r2 = system.bookRoom("MIT", "103", "2025-01-15", "09:00", 90);
  BookingResult r3 = system.bookRoom("Harvard", "102", "2025-01-15", "10:00", 60);
  BookingResult r4 = system.bookRoom("Stanford", "103", "2025-01-15", "11:00", 60);

  assert(r1.success);
  assert(r2.success);
  assert(r3.success);
  assert(r4.success);

  assert(system.getSchedule().size() == 4);
  std::cout << "✓ Multiple Institutes Multiple Rooms\n";
}

int main()
{
  std::cout << "=== UniversitySystem Tests ===\n";
  testAddInstitute();
  testAddRoom();
  testAssignRoom();
  testBookRoom();
  testBookingTimeConflict();
  testCancelBooking();
  testFindNonExistentBooking();
  testClearSystem();
  testCopySemantics();
  testZeroDurationBooking();
  testMultipleInstitutesAndRooms();
  std::cout << "All tests passed!\n";
  return 0;
}
