[Для файлового менеджера]
! если с авторского пк, то в main.c поменять путь к директориям (не предусмотрел ввод с клавиатуры)


[Чтобы сэкономить время]

gcc main.c create_wnds.c fill_catalogs.c movement.c print_catalogs.c -c

gcc main.o create_wnds.o fill_catalogs.o movement.o print_catalogs.o -o file_manager -lncurses

./file_manager
