#include <stdio.h>
#include <string.h>

int main() {
    // инициализируем массив для записи в файл и указатель на структуру FILE 
    char message[] = "String from file";
    FILE *myfile;
    
    // создаём/пересоздаём пустой файл для записи и проверяем открылся ли он(для себя: w+ для открытия, создания, чтения и записи)
    myfile = fopen("output.txt", "w");
    if (myfile == NULL) {
      perror("Не удалось открыть файл output.txt");
      return 1;
    } else {
        printf("Создан файл output.txt\n");
    }
    
    // записываем сообщение в файл и закрываем его
    fprintf(myfile, "%s", message);
    fclose(myfile);
    printf("Файл успешно закрыт\n");
    
    // переоткрываем файл, но уже для чтения и проверяем открылся ли он
    myfile = fopen("output.txt", "r");
    if (myfile == NULL) {
      perror("Не удалось открыть файл output.txt");
      return 1;
    } else {
        printf("Файл output.txt успешно открыт\n");
    }
    
    // инициализируем массив - буфер для чтения
    char buff[50] = "";
    
    // в цикле: с помощью fseek перемещаем указатель в конец файла, от конца файла перемещаемся на i-1 байт влево
    // и следующий символ, с помощью функции fgetc, записываем в buff. Таким образом, получаем запись с конца файла
    for (int i = 0; i < strlen(message); i++) {
      fseek(myfile, -i-1, SEEK_END);
      buff[i] = fgetc(myfile);
    }
    fclose(myfile); // зыкрываем файл
    printf("Файл успешно закрыт\n");
    
    // вывод результата на экран
    printf("%s\n", buff);
    printf("Дело сделано\n");
    
    return 0;
}
