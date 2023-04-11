#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    const char* input_file = "output.txt";
    const char* temp_file = "temp.txt";
    FILE* fp_in = fopen(input_file, "r");
    FILE* fp_out = fopen(temp_file, "w");
    char line[1000];

    if (fp_in == NULL || fp_out == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), fp_in)) {
        int i;
        for (i = 0; isspace(line[i]); i++); // skip whitespace
        if (line[i] != '\0') { // line is not empty
            fputs(line, fp_out);
        }
    }

    fclose(fp_in);
    fclose(fp_out);

    if (remove(input_file) != 0) {
        printf("Error deleting input file.\n");
        exit(1);
    }
    if (rename(temp_file, input_file) != 0) {
        printf("Error renaming temporary file.\n");
        exit(1);
    }

    printf("Empty lines removed successfully.\n");
    return 0;
}
