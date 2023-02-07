#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "linked_list.h"

#include "repo_experimentos.h"

void repoexp_salvaMatrizConfusaoCsv(const char *filename, HashTable *matriz,
                                    float accuracy) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
        exception_throw("IOException", "Couldn't open file for writing",
                        EXIT_FAILURE);

    fprintf(fp, "acc,%g\n", accuracy);
    fprintf(fp, "%c", '\n');

    LinkedList *classes = ht_keys(matriz);

    fprintf(fp, "true\\predict");

    char *row = NULL;
    void *saveptr = NULL;
    while ((row = ll_iter(classes, &saveptr)) != NULL)
        fprintf(fp, ",%s", row);

    fprintf(fp, "%c", '\n');

    saveptr = NULL;
    while ((row = ll_iter(classes, &saveptr)) != NULL) {
        fprintf(fp, "%s", row);

        char *column = NULL;
        void *saveptr = NULL;
        while ((column = ll_iter(classes, &saveptr)) != NULL) {
            HashTable *predictClasses = ht_get(matriz, row);
            int *value = ht_get(predictClasses, column);

            fprintf(fp, ",%d", value == NULL ? 0 : *value);
        }

        fprintf(fp, "%c", '\n');
    }

    ll_dispose(classes);

    fclose(fp);
}

void repoexp_salvaMatrizConfusaoTxt(const char *filename, HashTable *matriz,
                                    float accuracy) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL)
        exception_throw("IOException", "Couldn't open file for writing",
                        EXIT_FAILURE);

    fprintf(fp, "Accuracy: %g\n", accuracy);
    fprintf(fp, "%c", '\n');

    LinkedList *classes = ht_keys(matriz);

    fprintf(fp, "True\\Predict");

    char *row = NULL;
    void *saveptr = NULL;
    while ((row = ll_iter(classes, &saveptr)) != NULL) {
        fprintf(fp, " | %5s", row);
    }

    fprintf(fp, "%c", '\n');

    char c = '-';
    int count = 12;
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%c", c);
    }

    int n = ll_get_length(classes);
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s", "-|-");
        int count = 5;
        for (int j = 0; j < count; j++) {
            fprintf(fp, "%c", c);
        }
    }

    fprintf(fp, "%c", '\n');

    saveptr = NULL;
    while ((row = ll_iter(classes, &saveptr)) != NULL) {
        fprintf(fp, "%12s", row);

        char *column = NULL;
        void *saveptr = NULL;
        while ((column = ll_iter(classes, &saveptr)) != NULL) {
            HashTable *predictClasses = ht_get(matriz, row);
            int *value = ht_get(predictClasses, column);

            fprintf(fp, " | %5d", value == NULL ? 0 : *value);
        }

        fprintf(fp, "%c", '\n');
    }

    ll_dispose(classes);

    fclose(fp);
}
