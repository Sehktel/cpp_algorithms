# GDB скрипт для анализа переполнения буфера
#
# Использование:
#   gdb -x analyze_buffer.gdb ./buffer_overflow
#
# Или передать аргумент:
#   gdb -x analyze_buffer.gdb --args ./buffer_overflow "A"*100

# Установить аргументы (если не переданы через командную строку)
if $argc == 0
    set args "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
end

# Включить вывод ассемблерного кода
set disassembly-flavor intel

# Установить точку останова в уязвимой функции
break vulnerable_function

# Запустить программу
run

# Показать ассемблерный код функции
disassemble vulnerable_function

# Показать информацию о стеке
info frame

# Показать регистры
info registers

# Показать содержимое буфера (64 байта)
x/64xb $rsp

# Показать содержимое буфера как строку
x/s $rsp

# Продолжить выполнение
continue

# Показать информацию о стеке после переполнения
info stack

# Показать регистры после переполнения
info registers

# Показать адрес возврата
x/x $rsp+64

