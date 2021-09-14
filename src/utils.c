#include "../include/utils.h"
#include <string.h>

void utils_rtrim(char* str) {
    str[strcspn(str, "\n")] = 0;
}
