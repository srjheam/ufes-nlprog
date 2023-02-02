#include <libgen.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "exception.h"
#include "extlib.h"
#include "key_value_pair.h"
#include "palavra.h"
#include "ref_palavra.h"
#include "repo_noticias.h"

#include "indexador.h"

Documento *indexador_criaDocumento(char *train_instruc) {
    char *nome = NULL;
    char *classe = NULL;

    char *saveptr = NULL, *token = NULL;
    int i;
    for (i = 1, token = train_instruc;; i++, token = NULL) {
        token = strtok_r(token, " ", &saveptr);
        if (token == NULL)
            break;

        if (i == 1) {
            if ((nome = strdup(token)) == NULL)
                break;
        } else if (i == 2) {
            if ((classe = strdup(token)) == NULL)
                break;
        } else
            break;
    }

    if (i != 3) {
        return NULL;
    }

    FILE *fdoc = fopen(nome, "r");
    if (fdoc == NULL)
        exception_throw_failure(
            "Nao pode abrir noticia em indexador.indexador_criaDocumento");

    Documento *doc = reponoticias_carregaDocumento(fdoc, nome, classe);

    free(nome);
    free(classe);

    fclose(fdoc);

    return doc;
}

HashTable *indexador_criaIdxPalavras(HashTable *idxDocumentos) {
    // KeyValuePair<string, int*>
    HashTable *idxFreq =
        ht_init((hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)intdup,
                (cmp_fn)strcmp, (free_fn)free, (free_fn)free);

    // KeyValuePair<string, Documento>
    KeyValuePair *curr = NULL;
    int *saveptr = calloc(1, sizeof *saveptr);
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        Documento *doc = kvp_get_value(curr);

        // HashTable<string, RefPalavra>
        HashTable *palavras = doc_get_refPalavras(doc);

        // KeyValuePair<string, RefPalavra>
        KeyValuePair *curr_refpalavra = NULL;
        int *saveptr = calloc(1, sizeof *saveptr);
        while ((curr_refpalavra = ht_iter(palavras, saveptr)) != NULL) {
            RefPalavra *refpalavra = kvp_get_value(curr_refpalavra);
            char *palavra = refpalavra_get_palavra(refpalavra);

            int *freq = ht_get(idxFreq, palavra);
            if (freq == NULL) {
                int *v = malloc(sizeof *v);
                *v = 1;

                ht_insert(idxFreq, palavra, v);
                free(v);
            } else {
                *freq += 1;
            }
        }
        free(saveptr);
    }
    free(saveptr);

    // HashTable<string, Palavra>
    HashTable *idxPalavras =
        ht_init((hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)palavra_cpy,
                (cmp_fn)strcmp, (free_fn)free, (free_fn)palavra_dispose);

    saveptr = calloc(1, sizeof *saveptr);
    curr = NULL;
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        Documento *doc = kvp_get_value(curr);

        HashTable *palavras = doc_get_refPalavras(doc);

        // KeyValuePair<string, RefPalavra>
        KeyValuePair *curr_refpalavra = NULL;
        int *saveptr = calloc(1, sizeof *saveptr);
        while ((curr_refpalavra = ht_iter(palavras, saveptr)) != NULL) {
            RefPalavra *refpalavra = kvp_get_value(curr_refpalavra);
            char *palavra = refpalavra_get_palavra(refpalavra);
            int frequencia = refpalavra_get_freq(refpalavra);

            // calcula o tf-idf
            int n = ht_get_length(idxDocumentos);
            int *df = ht_get(idxFreq, palavra);

            float idf_dividendo = 1 + n;
            float idf_divisor = 1 + *df;

            float idf = log((idf_dividendo / idf_divisor)) + 1;

            float tfIdf = frequencia * idf;

            char *documento = doc_get_arquivo(doc);
            RefDocumento *refdocumento =
                refdoc_init(documento, frequencia, tfIdf);

            Palavra *pal = ht_get(idxPalavras, palavra);
            if (pal == NULL) {
                // HashTable<string, RefDocumento>
                HashTable *refdocumentos = ht_init(
                    (hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)refdoc_cpy,
                    (cmp_fn)strcmp, (free_fn)free, (free_fn)refdoc_dispose);

                ht_insert(refdocumentos, documento, refdocumento);

                refdoc_dispose(refdocumento);

                Palavra *nova_palavra = palavra_init(palavra, refdocumentos);

                ht_dispose(refdocumentos);

                ht_insert(idxPalavras, palavra, nova_palavra);

                palavra_dispose(nova_palavra);
            } else {
                // HashTable<string, RefDocumento>
                HashTable *refdocumentos = palavra_get_refDocumentos(pal);

                ht_insert(refdocumentos, documento, refdocumento);

                refdoc_dispose(refdocumento);
            }
        }
        free(saveptr);
    }
    free(saveptr);

    ht_dispose(idxFreq);

    return idxPalavras;
}

Indice *indexador_criaIndice(const char *trainPath) {
    // HashTable<string, Documento>
    HashTable *documentos =
        ht_init((hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)doc_cpy,
                (cmp_fn)strcmp, (free_fn)free, (free_fn)doc_dispose);

    FILE *train = fopen(trainPath, "r");
    if (train == NULL)
        exception_throw_failure("Falha ao abrir arquivo train.txt");

    char *traincpy = strdup(trainPath);
    char *basePath = dirname(traincpy);

    char *buffer = NULL;
    size_t len = 0;
    while (getline(&buffer, &len, train) > 1) {
        int pathSize = strlen(basePath) + strlen(buffer) + 1;

        // path + classe
        char *train_instruct = malloc(pathSize);
        snprintf(train_instruct, pathSize, "%s/%s", basePath, buffer);

        Documento *doc = indexador_criaDocumento(train_instruct);
        if (doc == NULL)
            continue;

        free(train_instruct);

        ht_insert(documentos, doc_get_arquivo(doc), doc);

        doc_dispose(doc);
    }
    free(buffer);
    free(traincpy);

    fclose(train);

    // HashTable<string, Palavra>
    HashTable *palavras = indexador_criaIdxPalavras(documentos);

    Indice *idx = indice_init(documentos, palavras);

    ht_dispose(documentos);
    ht_dispose(palavras);

    return idx;
}
