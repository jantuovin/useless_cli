#include <stdbool.h>
#include <stdlib.h>

bool is_str_equal(char *str1, char *str2);
char *find_option_value(int argc, char **argv, char *option_key);
unsigned long djb2_hash(char *str);
