#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

double tfidf[MAX_DOCS][MAX_WORDS];

void read_words(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: could not open file %s\\n", filename);
        exit(1);
    }
    char word[100];
    while (fgets(word, 100, fp) != NULL) {
        // Remove trailing newline character
        word[strcspn(word, "\\n")] = 0;
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
        printf("Error: could not open file %s\\n", filename);
        exit(1);
    }
    char doc_name[100];
    while (fscanf(fp, "%s", doc_name) != EOF) {
        doc_names[num_docs++] = strdup(doc_name);
        read_words(doc_name);
    }
    fclose(fp);
}

void calculate_tfidf() {
    int i, j;
    for (i = 0; i < num_docs; i++) {
        for (j = 0; j < num_words; j++) {
            double tf = (double)doc_word_counts[i][j] / doc_lengths[i];
            double idf = log((double)num_docs / word_doc_counts[j]);
            tfidf[i][j] = tf * idf;
        }
    }
}

void print_tfidf() {
    int i, j;
    for (i = 0; i < num_docs; i++) {
        printf("%s: ", doc_names[i]);
        for (j = 0; j < num_words; j++) {
            printf("%s:%f ", words[j], tfidf[i][j]);
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
    calculate_tfidf();
    print_tfidf();
    return 0;
}
