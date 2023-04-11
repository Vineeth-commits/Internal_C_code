#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define MAX_MATCHES 100

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s input_file output_file\n", argv[0]);
        return 1;
    }

    const char* pattern = "\\S+|.";  // match non-space characters or a single character
    regex_t regex;
    int reti = regcomp(&regex, pattern, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile regex\n");
        return 1;
    }

    FILE *fp_in = fopen(argv[1], "r");
    if (!fp_in) {
        fprintf(stderr, "Could not open input file\n");
        return 1;
    }

    FILE *fp_out = fopen(argv[2], "w");
    if (!fp_out) {
        fprintf(stderr, "Could not open output file\n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp_in)) != -1) {
        regmatch_t matches[MAX_MATCHES];
        int n_matches = 0;
        char *str = line;

        while (!regexec(&regex, str, MAX_MATCHES, matches, 0)) {
            n_matches = matches[0].rm_eo - matches[0].rm_so;
            char token[n_matches + 1];
            memcpy(token, &str[matches[0].rm_so], n_matches);
            token[n_matches] = '\0';
            fprintf(fp_out, "%s\n", token);
            str += matches[0].rm_eo;
        }
    }

    fclose(fp_in);
    fclose(fp_out);
    if (line) free(line);
    regfree(&regex);

    return 0;
}
