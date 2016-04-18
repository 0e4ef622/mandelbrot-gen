#include <stdio.h>
#include <errno.h>

void errno_msg(char *filename) {
    fprintf(stderr, "Error opening file: %s\n", filename);
    switch (errno) {
        case EACCES:
            fprintf(stderr, "Permission denied\n");
            break;
        case ENOENT:
            fprintf(stderr, "No such file\n");
            break;
        case ENOTDIR:
            fprintf(stderr, "Not a directory\n");
            break;
        default:
            fprintf(stderr, "Something went wrong and I don't know what\n");
    }
}
