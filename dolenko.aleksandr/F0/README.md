# University Classroom Booking System

Система управления расписанием и бронированием аудиторий для университета.

## Быстрый старт

### Способ 1: Через скрипт (рекомендуется)

```bash
cd /Users/aleksandrdolenko/Desktop/spbspu-aads-2026-5130904-50004/dolenko.aleksandr/F0
./build.sh
```

### Способ 2: Прямая компиляция

```bash
cd /Users/aleksandrdolenko/Desktop/spbspu-aads-2026-5130904-50004/dolenko.aleksandr/F0
clang++ -std=c++17 -Wall -Wextra main.cpp credentials.cpp UniversitySystem.cpp -o booking_system
./booking_system
```

### Способ 3: Запуск уже скомпилированного файла

```bash
/Users/aleksandrdolenko/Desktop/spbspu-aads-2026-5130904-50004/dolenko.aleksandr/F0/booking_system
```

## Исходные данные

При запуске программа автоматически инициализируется с данными:

**Институты:**
- MIT
- Stanford
- Harvard
- Berkeley
- Carnegie Mellon

**Комнаты:**
- 101, 102 (назначены MIT)
- 201, 202 (назначены Stanford)
- 301 (назначена Harvard)
- 302 (назначена Berkeley)

## Доступные команды

- `add_institute <name>` - Добавить новый институт
- `add_room <number>` - Добавить новую аудиторию
- `assign_room <room_number> <institute_name>` - Назначить аудиторию институту
- `book_room <institute> <room> <date> <time> <duration>` - Забронировать аудиторию
- `cancel_booking <booking_id>` - Отменить бронирование
- `find_booking <booking_id>` - Найти бронирование по ID
- `show_schedule <type> <name> <date>` - Показать расписание (room/institute)
- `free_rooms <institute> <date> <time> <duration>` - Найти свободные аудитории
- `clear` - Очистить все данные
- `exit` - Выход

## Примеры использования

```
book_room MIT 101 2025-09-21 10:00 90
show_schedule institute MIT 2025-09-21
free_rooms MIT 2025-09-21 11:45 45
find_booking 1
cancel_booking 1
```

## Архитектура

- **AVL-дерево** для глобального хранения расписания с защитой от конфликтов времени
- **Двусвязный список** для хранения институтов и аудиторий (BiList из S1)
- **Безопасность памяти**: std::unique_ptr для всех динамических аллокаций
- **Идиома copy-and-swap** для сильной гарантии исключений
- **Разделение логики и интерфейса**: UniversitySystem (бизнес-логика) отделена от CommandParser (I/O)

## Требования

- C++17 или новее
- clang++ или другой совместимый компилятор
