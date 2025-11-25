# GDB скрипт для отладки simple_debug.cpp
#
# Использование:
#   gdb -x debug_simple.gdb ./simple_debug
#
# Или в интерактивном режиме:
#   (gdb) source debug_simple.gdb

# Установить аргументы командной строки
set args "Очень длинная строка для тестирования переполнения буфера"

# Установить точки останова
break main
break process_string
break calculate_sum
break factorial

# Запустить программу
run

# Показать информацию о стеке вызовов
info stack

# Показать локальные переменные
info locals

# Показать аргументы функции
info args

# Продолжить выполнение до следующей точки останова
continue

# Показать код вокруг текущей позиции
list

# Показать регистры
info registers

# Показать память (первые 32 байта стека)
x/32xb $rsp

