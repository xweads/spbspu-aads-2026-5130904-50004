#include <iostream>
#include <cassert>
#include "ScheduleAVL.hpp"
#include "Booking.hpp"

using namespace dolenko;

void testAVLInsertAndFind()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  Booking b2(2, "MIT", "102", "2025-01-15", "10:00", 90);
  Booking b3(3, "MIT", "103", "2025-01-15", "11:00", 120);

  assert(avl.insert(b1));
  assert(avl.insert(b2));
  assert(avl.insert(b3));

  assert(avl.size() == 3);
  assert(avl.contains(b1));
  assert(avl.contains(b2));
  assert(avl.contains(b3));
  std::cout << "✓ AVL Insert and Find\n";
}

void testAVLDelete()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  Booking b2(2, "MIT", "102", "2025-01-15", "10:00", 90);

  avl.insert(b1);
  avl.insert(b2);

  assert(avl.size() == 2);
  assert(avl.remove(b1));
  assert(avl.size() == 1);
  assert(!avl.contains(b1));
  assert(avl.contains(b2));
  std::cout << "✓ AVL Delete\n";
}

void testAVLDuplicateInsert()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);

  assert(avl.insert(b1));
  assert(!avl.insert(b1));  // Duplicate should fail
  assert(avl.size() == 1);
  std::cout << "✓ AVL Duplicate Insert\n";
}

void testAVLIterator()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  Booking b2(2, "MIT", "102", "2025-01-15", "10:00", 90);
  Booking b3(3, "MIT", "103", "2025-01-15", "08:00", 60);

  avl.insert(b1);
  avl.insert(b2);
  avl.insert(b3);

  int count = 0;
  for (auto it = avl.begin(); it != avl.end(); ++it)
  {
    ++count;
    assert((*it).id >= 1 && (*it).id <= 3);
  }
  assert(count == 3);
  std::cout << "✓ AVL Iterator In-Order Traversal\n";
}

void testAVLConstIterator()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  avl.insert(b1);

  const ScheduleAVL< Booking >& const_avl = avl;
  for (auto it = const_avl.begin(); it != const_avl.end(); ++it)
  {
    assert((*it).id == 1);
  }
  std::cout << "✓ AVL Const Iterator\n";
}

void testBookingComparison()
{
  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  Booking b2(2, "MIT", "102", "2025-01-15", "10:00", 60);
  Booking b3(1, "MIT", "101", "2025-01-15", "09:00", 60);

  assert(b1 < b2);
  assert(!(b2 < b1));
  assert(b1 == b3);
  assert(b1 != b2);
  std::cout << "✓ Booking Comparison Operators\n";
}

void testAVLCopy()
{
  ScheduleAVL< Booking > avl1;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  Booking b2(2, "MIT", "102", "2025-01-15", "10:00", 90);

  avl1.insert(b1);
  avl1.insert(b2);

  ScheduleAVL< Booking > avl2 = avl1;

  assert(avl2.size() == 2);
  assert(avl2.contains(b1));
  assert(avl2.contains(b2));

  Booking b3(3, "MIT", "103", "2025-01-15", "11:00", 60);
  avl1.insert(b3);

  assert(avl1.size() == 3);
  assert(avl2.size() == 2);
  std::cout << "✓ ScheduleAVL Copy Constructor\n";
}

void testAVLMove()
{
  ScheduleAVL< Booking > avl1;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  avl1.insert(b1);

  ScheduleAVL< Booking > avl2 = std::move(avl1);

  assert(avl2.size() == 1);
  assert(avl1.size() == 0);
  assert(avl2.contains(b1));
  std::cout << "✓ ScheduleAVL Move Constructor\n";
}

void testAVLClear()
{
  ScheduleAVL< Booking > avl;

  Booking b1(1, "MIT", "101", "2025-01-15", "09:00", 60);
  avl.insert(b1);

  assert(avl.size() == 1);
  avl.clear();
  assert(avl.size() == 0);
  assert(avl.empty());
  std::cout << "✓ ScheduleAVL Clear\n";
}

int main()
{
  std::cout << "=== AVL Tree and Booking Tests ===\n";
  testAVLInsertAndFind();
  testAVLDelete();
  testAVLDuplicateInsert();
  testAVLIterator();
  testAVLConstIterator();
  testBookingComparison();
  testAVLCopy();
  testAVLMove();
  testAVLClear();
  std::cout << "All tests passed!\n";
  return 0;
}
