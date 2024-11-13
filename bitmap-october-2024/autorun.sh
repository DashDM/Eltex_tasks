#!/usr/bin/expect -f

if { $argc != 1 } {
    puts "Usage: ./autotest.sh <path_to_executable>"
    exit 1
}

set executable [lindex $argv 0];
set delay 0.1

# Запускаем приложение
spawn $executable

expect "Выбор плагинов"

# Подключим плагины
send "1\r"
sleep $delay
send "2\r"
sleep $delay
send "3\r"
sleep $delay
send "4\r"
sleep $delay
send "5\r"
sleep $delay

# Добавим первый битмап
send "1\r"
sleep $delay
send "20\r"
sleep $delay
send "1010111101110\r"
sleep $delay

# Добавим второй битмап
send "1\r"
sleep $delay
send "30\r"
sleep $delay
send "10111011010101010\r"
sleep $delay

# Добавим третий битмап
send "1\r"
sleep $delay
send "30\r"
sleep $delay
send "10111011010101010\r"
sleep $delay

# Выведем список битмапов
send "3\r"
sleep $delay

# Изменим один из битмапов
send "4\r"
sleep $delay
send "0\r"
sleep $delay
send "19\r"
sleep $delay
send "1\r"
sleep $delay

# Проведем тестирование операций сравнения
send "6\r"
sleep $delay
send "1\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "6\r"
sleep $delay
send "2\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "6\r"
sleep $delay
send "3\r"
sleep $delay
send "1\r"
sleep $delay
send "2\r"
sleep $delay

send "6\r"
sleep $delay
send "4\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "6\r"
sleep $delay
send "5\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

# Проведем клонирование
send "8\r"
sleep $delay
send "0\r"
sleep $delay

# Выведем список битмапов
send "3\r"
sleep $delay

# Проведем склеивание
send "9\r"
sleep $delay
send "2\r"
sleep $delay
send "3\r"
sleep $delay

# Проведем копирование
send "7\r"
sleep $delay
send "2\r"
sleep $delay
send "16\r"
sleep $delay
send "8\r"
sleep $delay
send "4\r"
sleep $delay
send "8\r"
sleep $delay

# Выведем список битмапов
send "3\r"
sleep $delay

# Проведем сброс
send "10\r"
sleep $delay
send "4\r"
sleep $delay

# Проведем удаление
send "2\r"
sleep $delay
send "4\r"
sleep $delay

# Выведем список битмапов
send "3\r"
sleep $delay

# Проведем тестирование побитовых операций
expect "Выберите операцию: "
send "5\r"
sleep $delay
send "1\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "5\r"
sleep $delay
send "2\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "5\r"
sleep $delay
send "3\r"
sleep $delay
send "0\r"
sleep $delay
send "1\r"
sleep $delay

send "5\r"
sleep $delay
send "4\r"
sleep $delay
send "2\r"
sleep $delay

# Выведем список битмапов
expect "Выберите пункт меню"
send "3\r"
sleep $delay

# Выйдем из программы
expect "<bitmap 6>"
send "11\r"

expect eof