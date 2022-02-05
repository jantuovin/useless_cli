#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#include "../Headers/util.h"

#define ITEM_MAX_LENGTH 256
#define ITEM_FILE "items.data"
#define TMP_ITEM_FILE "items_tmp.data"

int help_action(void)
{
    char *help =
        "Usage: useless COMMAND [OPTIONS]\n\n"
        "Commands:\n"
        "  add        Add an item to the file\n"
        "  delete     Delete an item from the file\n"
        "  print      Print items on the file\n\n"
        "To get more help for a command: useless COMMAND --help\n";

    puts(help);

    return 0;
}

int add_action(int argc, char **argv)
{
    char *help_option = find_option_value(argc, argv, "--help");

    if (is_str_equal(help_option, ""))
    {
        char *help =
            "\n\n"
            "Usage: useless add [OPTIONS]\n\n"
            "Add item to the file\n\n"
            "Options:\n"
            "  -t      Item type,  accepted types: N=number, S=string, *required\n"
            "  -v      Item value, max length: 10 chars,               *required\n";

        puts(help);
        return 0;
    }

    char *item_type = find_option_value(argc, argv, "-t");
    char *item_value = find_option_value(argc, argv, "-v");

    if (
        !(is_str_equal(item_type, "N") || is_str_equal(item_type, "S"))
        || is_str_equal(item_value, "") || 10 < strlen(item_value))
    {
        char *info =
            "See useless add --help\n\n"
            "Add item to the file";

        puts(info);
        return 0;
    }

    char item_str_base[ITEM_MAX_LENGTH];
    unsigned long unix_epoch = (unsigned long)time(NULL);

    if (is_str_equal(item_type, "N"))
    {
        sprintf(item_str_base, "%s %f %lu", item_type, atof(item_value), unix_epoch);
    } else
    {
        sprintf(item_str_base, "%s %s %lu", item_type, item_value, unix_epoch);
    }

    unsigned long item_hash = djb2_hash(item_str_base);

    char item_str[ITEM_MAX_LENGTH + 32];
    sprintf(item_str, "%lu %s", item_hash, item_str_base);

    FILE *file = fopen(ITEM_FILE, "a");
    if (file == NULL)
    {
        return 0;
    }

    fprintf(file, "%s\n", item_str);
    fclose(file);

    return 0;
}

int delete_action(int argc, char **argv)
{
    char *help_option = find_option_value(argc, argv, "--help");

    if (is_str_equal(help_option, ""))
    {
        char *help =
            "\n\n"
            "Usage: useless delete [item hash]\n\n"
            "Delete an item to the file\n\n";

        puts(help);
        return 0;
    }

    if (argc < 3)
    {
        char *info =
            "See useless delete --help\n\n"
            "Delete an item from the file";

        puts(info);
        return 0;
    }

    FILE *file = fopen(ITEM_FILE, "r");
    FILE *tmp_file = fopen(TMP_ITEM_FILE, "w");
    if (file == NULL || tmp_file == NULL)
    {
        return 0;
    }

    char *item_hash = argv[2];

    unsigned long int hash;
    char type[2];
    char value[11];
    unsigned long int time;
    int result;
    while ((result = fscanf(file, "%10lu %2s %11s %10lu\n", &hash, type, value, &time)) != EOF)
    {
        if (result != 4 || (item_hash != NULL && strtoul(item_hash, NULL, 10) == hash))
        {
            continue;
        }

        fprintf(tmp_file, "%lu %s %s %lu\n", hash, type, value, time);
    }

    fclose(file);
    fclose(tmp_file);

    if (remove(ITEM_FILE) != 0)
    {
        return 0;
    }

    rename(TMP_ITEM_FILE, ITEM_FILE);

    return 0;
}

int print_action(int argc, char **argv)
{
    char *help_option = find_option_value(argc, argv, "--help");

    if (is_str_equal(help_option, ""))
    {
        char *help =
            "\n\n"
            "Usage: useless print [OPTIONS]\n\n"
            "Print items in the file\n\n"
            "Options:\n"
            "  -t      Type of printed items,                       *optional\n"
            "  -s      Starting from timestamp, format: unix epoch, *optional\n"
            "  -e      Ending to timestamp,     format: unix epoch, *optional\n";

        puts(help);
        return 0;
    }

    FILE *file = fopen(ITEM_FILE, "r");
    if (file == NULL)
    {
        return 0;
    }

    char *item_type = find_option_value(argc, argv, "-t");
    char *starting_from = find_option_value(argc, argv, "-s");
    char *ending_to = find_option_value(argc, argv, "-e");

    unsigned long int hash;
    char type[2];
    char value[11];
    unsigned long int time;
    int result;
    while ((result = fscanf(file, "%10lu %2s %11s %10lu\n", &hash, type, value, &time)) != EOF)
    {
        if (
            result != 4
            || (item_type != NULL && strcmp(item_type, type))
            || (starting_from != NULL && time < strtol(starting_from, NULL, 10))
            || (ending_to != NULL && strtol(ending_to, NULL, 10) < time))
        {
            continue;
        }

        printf("%lu %s %s %lu\n", hash, type, value, time);
    }

    fclose(file);

    return 0;
}
