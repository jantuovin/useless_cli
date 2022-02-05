#include <stdio.h>

#include "../Headers/util.h"
#include "../Headers/action.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        return help_action();
    }

    char *command = argv[1];

    if (is_str_equal(command, "add"))
    {
        return add_action(argc, argv);
    }
    else if (is_str_equal(command, "delete"))
    {
        return delete_action(argc, argv);
    }
    else if (is_str_equal(command, "print"))
    {
        return print_action(argc, argv);
    }
    else
    {
        printf("Couldn't find command: %s\n\n", command);
        return help_action();
    }
}
