#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "cbfc.h"

static int spaces = 4;

static int cbfc_convert_format(FILE *fp, const char *str) {
    return fprintf(fp, "%*c%s\n", spaces, ' ', str);
}

int cbfc_convert(const char *fp_name, FILE *fp, FILE *error_output) {
    FILE *out, *tmp;
    char *output_name;
    int c;
    size_t pos = 1;

    output_name = (char *)malloc(sizeof(*output_name) * (strlen(fp_name) + 1 + 2));

    if (output_name == NULL) {
        return 0;
    }

    strcpy(output_name, fp_name);
    strcat(output_name, ".c");

    if ((tmp = fopen(output_name, "w")) == NULL) {
        free(output_name);
        return 0;
    }

    fclose(tmp);

    out = fopen(output_name, "a");

    if (out == NULL) {
        free(output_name);
        return 0;
    }

    fputs("#include <stdio.h>\n\nint main(void) {\n", out);
    cbfc_convert_format(out, "char array[30000] = {0};");
    cbfc_convert_format(out, "char *ptr = &array[0];");

    while ((c = fgetc(fp)) != EOF) {
        if (isspace(c)) {
            continue;
        }

        switch (c) {
            case '>': cbfc_convert_format(out, "++ptr;"); break;
            case '<': cbfc_convert_format(out, "--ptr;"); break;
            case '+': cbfc_convert_format(out, "++*ptr;"); break;
            case '-': cbfc_convert_format(out, "--*ptr;"); break;
            case '.': cbfc_convert_format(out, "putchar(*ptr);"); break;
            case ',': cbfc_convert_format(out, "*ptr = getchar();"); break;
            case '[': cbfc_convert_format(out, "while (*ptr) {"); spaces += 4; break;
            case ']': spaces -= 4; cbfc_convert_format(out, "}"); break;
            default:
                fprintf(error_output, "cbfc runtime error: File \"%s\", Invalid character '%c' at position %zu\n",
                        fp_name, c, pos);
                fflush(stderr);
                c = EOF;
                break;
        }
        pos++;
    }

    cbfc_convert_format(out, "return 0;");
    fputs("}\n", out);

    fclose(out);
    free(output_name);

    return 1;
}
