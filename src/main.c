#include "lib.h"
#include <dirent.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_FLAGS 3

void recursive_search(const char* path, int* sort_flags, int sort_flags_number,
                      int sort_output)
{
    struct dirent** namelist;
    int num_entries;

    if (sort_output) {
        num_entries = scandir(path, &namelist, NULL, alphasort);
    }
    else {
        num_entries = scandir(path, &namelist, NULL, NULL);
    }

    if (num_entries < 0) {
        perror("scandir");
        return;
    }

    for (int i = 0; i < num_entries; i++) {
        if (strcmp(namelist[i]->d_name, ".") == 0
            || strcmp(namelist[i]->d_name, "..") == 0) {
            continue;
        }

        char full_path[PATH_MAX];

        if (strcmp(path, "/") == 0) {
            snprintf(full_path, sizeof(full_path), "/%s", namelist[i]->d_name);
        }
        else {
            snprintf(full_path, sizeof(full_path), "%s/%s", path,
                     namelist[i]->d_name);
        }

        if (sort_flags_number > 0) {
            for (int j = 0; j <= sort_flags_number; j++) {
                if (sort_flags[j] == namelist[i]->d_type) {
                    printf("%s\n", full_path);
                }
            }
        }
        else {
            printf("%s\n", full_path);
        }

        if (namelist[i]->d_type == DT_DIR) {
            recursive_search(full_path, sort_flags, sort_flags_number,
                             sort_output);
        }

        free(namelist[i]);
    }

    free(namelist);
}

int main(int argc, char** argv)
{
    struct gengetopt_args_info args_info;
    char* name;
    int arr[MAX_FLAGS];
    int flags_num = 0;
    int sort_output;

    if (cmdline_parser(argc, argv, &args_info) != 0) exit(EXIT_FAILURE);

    if (args_info.inputs_num > 1) {
        fprintf(stderr, "Only one path can be used\n");
    }

    if (args_info.inputs_num == 0) { name = "."; }
    else {
        name = args_info.inputs[0];
    }

    for (int i = 0; i < MAX_FLAGS; i++) { arr[i] = 0; }

    if (args_info.dirs_flag == 1) {
        arr[0] = DT_DIR;
        flags_num++;
    }
    if (args_info.files_flag == 1) {
        arr[1] = DT_REG;
        flags_num++;
    }
    if (args_info.links_flag == 1) {
        arr[2] = DT_LNK;
        flags_num++;
    }

    sort_output = args_info.sort_flag;

    if (strlen(name) > 1 && name[strlen(name) - 1] == '/') {
        name[strlen(name) - 1] = '\0';
    }

    recursive_search(name, arr, flags_num, sort_output);

    cmdline_parser_free(&args_info);

    return 0;
}
