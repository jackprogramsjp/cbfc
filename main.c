#include <stdio.h>
#include "cbfc.h"

int main(int argc, char **argv) {
    FILE *fp;

    if (argc != 2) {
        fputs("usage: cbfc [file]\n", stdout);
        return 0;
    }

    fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        fprintf(stderr, "file error: file \"%s\" cannot be read\n", argv[1]);
        return 0;
    }

    if (!cbfc_convert(argv[1], fp, stderr)) {
        fputs("cbfc error: Failed to convert bf file\n", stderr);
    }

    fclose(fp);

    return 0;
}
