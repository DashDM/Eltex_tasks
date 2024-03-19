#include "manager_functional.h"

//блок записи информации из каталогов (имя файла и размер файла в каталоге)
void fill_catalog(struct catalog **c_wnd, int *c_w_size, char *directory) {
    int count = 0; // для подсчёта содержимого папки
    
    DIR* dir; // указатель на поток к папке
    struct dirent *ent; // структура данных папки
    struct stat file_stat; // для информации о файле (чтобы узнать размер)
    
    dir = opendir(directory);
    while ((ent = readdir(dir)) != false) {
        stat(ent -> d_name, &file_stat);
        count++;
    }
    rewinddir(dir); // чтобы сбросить указатель
    *c_wnd = realloc(*c_wnd, count * sizeof(struct catalog));
    for (int i = 0; i < count; i++) {
        ent = readdir(dir);
        stat(ent -> d_name, &file_stat);
        strcpy((*c_wnd)[i].file_name, ent -> d_name);
        (*c_wnd)[i].file_size = file_stat.st_size;
    }
    
    *c_w_size = count;
}
