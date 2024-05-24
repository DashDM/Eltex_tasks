#include "operations_third.h"
// main интерпритатора
void my_bash(void) {
    char input_string[MAX_INPUT_LENGTH];
    
    while (1) {
        my_bash_input(input_string);
        execute_command(input_string);
    }
}
