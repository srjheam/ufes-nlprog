#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
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

    Documento *doc = reponoticias_carregaDocumento(fdoc, nome, classe);

    fclose(fdoc);

    return doc;
}

HashTable *indexador_criaIdxPalavras(HashTable *idxDocumentos) {
    // KeyValuePair<string, int*>
    HashTable *idxFreq =
        ht_init((cpy_fn)strdup, (cmp_fn)strcmp, (free_fn)free, (free_fn)free);

    // KeyValuePair<string, Documento>
    KeyValuePair *curr = NULL;
    int *saveptr = NULL;
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        Documento *doc = kvp_get_value(curr);

        // HashTable<string, RefPalavra>
        HashTable *palavras = doc_get_refPalavras(doc);

        // KeyValuePair<string, RefPalavra>
        KeyValuePair *curr_refpalavra = NULL;
        int *saveptr = NULL;
        while ((curr_refpalavra = ht_iter(palavras, saveptr)) != NULL) {
            RefPalavra *refpalavra = kvp_get_value(curr_refpalavra);
            char *palavra = refpalavra_get_palavra(refpalavra);

            int *freq = ht_get(idxFreq, palavra);
            if (freq == NULL) {
                int *v = malloc(sizeof *v);
                *v = 1;

                ht_add(idxFreq, palavra, v);
            } else {
                *freq++;
            }
        }
    }

    // KeyValuePair<string, Palavra>
    HashTable *idxPalavras = ht_init((cpy_fn)strdup, (cmp_fn)strcmp,
                                     (free_fn)free, (free_fn)palavra_dispose);

    saveptr = NULL;
    curr = NULL;
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        // KeyValuePair<string, Documento>
        Documento *doc = kvp_get_value(curr);

        KeyValuePair *curr_refpalavra = NULL;
        HashTable *palavras = doc_get_refPalavras(doc);

        while ((curr_refpalavra = ht_iter(palavras, saveptr)) != NULL) {
            RefPalavra *refpalavra = kvp_get_value(curr_refpalavra);
            char *palavra = refpalavra_get_palavra(refpalavra);
            int frequencia = refpalavra_get_freq(refpalavra);

            // calcula o tf-idf
            int n = ht_get_qty(curr);
            int df = ht_get(idxFreq, palavra);

            float idf_dividendo = 1 + n;
            float idf_divisor = 1 + df;

            float idf = log((idf_dividendo / idf_divisor) + 1);

            float tfIdf = frequencia * idf;

            RefDocumento *refdocumento = refdoc_init(curr, frequencia, tfIdf);

            Palavra *pal = ht_get(idxPalavras, palavra);
            if (pal == NULL) {
                Palavra *nova_palavra = palavra_init(palavra, refdocumento);
                ht_add(idxPalavras, nova_palavra, refdocumento);
            }

            /* else
                atualizar o refdocumento da palavra ja existente na hashtable */
        }
    }

    return idxPalavras;
}

Indice *indexador_criaIndice(FILE *train) {
    // HashTable<string, Documento>
    HashTable *documentos = ht_init((cpy_fn)strdup, (cmp_fn)strcmp,
                                    (free_fn)free, (free_fn)doc_dispose);

    char *buffer = NULL;
    size_t len = 0;
    while (getline(&buffer, &len, train) > 1) {
        Documento *doc = indexador_criaDocumento(buffer);
        ht_add(documentos, doc_get_arquivo(doc), doc);
    }
    free(buffer);

    // HashTable<string, Palavra>
    HashTable *palavras = indexador_criaIdxPalavras(documentos);

    Indice *idx = indice_init(documentos, palavras);

    return idx;
}
