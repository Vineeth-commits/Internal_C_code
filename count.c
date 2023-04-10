#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 100000
#define MAX_DOC_LENGTH 100000
#define MAX_DOCS 1000

char *words[MAX_WORDS];
int num_words = 0;

char *doc_names[MAX_DOCS];
int num_docs = 0;

int doc_word_counts[MAX_DOCS][MAX_WORDS];
int doc_lengths[MAX_DOCS] = {0};

int word_doc_counts[MAX_WORDS] = {0};

int count_vector[MAX_DOCS][MAX_WORDS];
void read_words(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\\n", filename);
        exit(1);
    }
    char word[100];
    while (fscanf(fp, "%s", word) != EOF) {
        int i;
        for (i = 0; i < num_words; i++) {
            if (strcmp(words[i], word) == 0) {
                break;
            }
        }
        if (i == num_words) {
            words[num_words++] = strdup(word);
        }
        doc_word_counts[num_docs][i]++;
        word_doc_counts[i]++;
        doc_lengths[num_docs]++;
    }
    fclose(fp);
}
void read_docs(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\\\\n", filename);
        exit(1);
    }
    char doc_name[100];
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        // Remove trailing newline character
        line[strcspn(line, "\\\\n")] = 0;
        doc_names[num_docs++] = strdup(line);
        read_words(line);
    }
    fclose(fp);
    if (line) {
        free(line);
    }
}

void calculate_count_vector() {
    int i, j;
    for (i = 0; i < num_docs; i++) {
        for (j = 0; j < num_words; j++) {
            count_vector[i][j] = doc_word_counts[i][j];
        }
    }
}

void print_count_vector() {
    int i, j;
    for (i = 0; i < num_docs; i++) {
        printf("%s: ", doc_names[i]);
        for (j = 0; j < num_words; j++) {
            printf("%s:%d ", words[j], count_vector[i][j]);
        }
        printf("\\n");
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <input file>\\n", argv[0]);
        exit(1);
    }
    read_docs(argv[1]);
    calculate_count_vector();
    print_count_vector();
    return 0;
}
