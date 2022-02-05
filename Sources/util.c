#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_str_equal(char *str1, char *str2)
{
    if (str1 == NULL || str2 == NULL) {
        return false;
    }
    return (strcmp(str1, str2) == 0);
}

char *find_option_value(int argc, char **argv, char *option_key)
{
    if (argc < 3)
    {
        return NULL;
    }

    bool option_found = false;
    char *option_value = NULL;

    for (int i = 2; i < argc; i++)
    {
        if (is_str_equal(option_key, argv[i]))
        {
            option_found = true;
        }

        if (option_found && i + 1 < argc)
        {
            option_value = argv[i + 1];
        }

        if (option_found) {
            break;
        }
    }

    if (!option_found)
    {
        return NULL;
    }

    if (option_value == NULL) {
        return "";
    }

    return option_value;
}

unsigned long djb2_hash(char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}
