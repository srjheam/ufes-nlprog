#include <stdlib.h>
#include <string.h>

#include "extlib.h"
#include "hash_table.h"
#include "indexador.h"
#include "palavra.h"

#include "classificador.h"

static int anonfn_sortKvpStrFloatDesc(const KeyValuePair *a,
                                      const KeyValuePair *b) {
    return *(float *)kvp_get_value(b) - *(float *)kvp_get_value(a);
}

static int anonfn_sortKvpStrIntDesc(const KeyValuePair *a,
                                    const KeyValuePair *b) {
    return *(int *)kvp_get_value(b) - *(int *)kvp_get_value(a);
}

char *classificador_classificaDocumento(Documento *documento, Indice *idx,
                                        int k) {
    char *nomeDocumento = doc_get_arquivo(documento);

    // HashTable<string, Documento>
    HashTable *docsIdx = indice_get_documentos(idx);
    ht_add(docsIdx, doc_get_arquivo(documento), documento);

    // HashTable<string, Palavra>
    HashTable *idxPalavras = indexador_criaIdxPalavras(docsIdx);
    // Atualiza o calculo do indice de palavras com a entrada do novo documento
    // no corpus

    // HashTable<string, float>
    HashTable *similaridadeDocumentos =
        ht_init((cpy_fn)strdup, (cpy_fn)floatdup, (cmp_fn)strcmp, (free_fn)free,
                (free_fn)free);
    // Associa identificador de um documento no indice com a similaridade entre
    // ele e o ducumento a ser classificado

    float moduloDocumento = 0;
    // KeyValuePair<string, RefPalavra>
    KeyValuePair *currPalavra = NULL;
    int saveptr = 0;
    while ((currPalavra = ht_iter(doc_get_refPalavras(documento), &saveptr)) !=
           NULL) {
        char *palavra = refpalavra_get_palavra(currPalavra);

        float tfIdf = refdoc_get_tdIdf(
            ht_get(palavra_get_refDocumentos(ht_get(idxPalavras, palavra)),
                   nomeDocumento));

        moduloDocumento += pow(tfIdf, 2);
    }

    moduloDocumento = sqrt(moduloDocumento);

    // KeyValuePair<string, Documento>
    KeyValuePair *curr = NULL;
    saveptr = 0;
    while ((curr = ht_iter(docsIdx, &saveptr)) != NULL) {
        Documento *doc = kvp_get_value(curr);
        char *currDocNome = doc_get_arquivo(doc);

        if (strcmp(currDocNome, nomeDocumento) == 0)
            continue;

        float similaridadeDoc = 0;
        float normaDoc = 0;

        // KeyValuePair<string, RefPalavra>
        KeyValuePair *currPalavra = NULL;
        int saveptr = 0;
        while ((currPalavra = ht_iter(doc_get_refPalavras(doc), &saveptr)) !=
               NULL) {
            // tfIdf do documento a ser classificado
            float documentoTfIdf = refdoc_get_tdIdf(
                ht_get(palavra_get_refDocumentos(
                           ht_get(idxPalavras, kvp_get_key(currPalavra))),
                       nomeDocumento));

            // tfIdf do documento atual
            float currDocTfIdf = refdoc_get_tdIdf(
                ht_get(palavra_get_refDocumentos(
                           ht_get(idxPalavras, kvp_get_key(currPalavra))),
                       currDocNome));

            similaridadeDoc += documentoTfIdf * currDocTfIdf;
            normaDoc += pow(currDocTfIdf, 2);
        }

        normaDoc = sqrt(normaDoc);
        similaridadeDoc /= (moduloDocumento * normaDoc);

        // Adiciona a similaridade calculada ao hash de similaridade
        ht_add(similaridadeDocumentos, currDocNome, &similaridadeDoc);
    }

    ht_dispose(idxPalavras);

    // Lista<KeyValuePair<string, float>>
    Lista *listaSimilaridade = ht_toList(similaridadeDocumentos);

    ht_dispose(similaridadeDocumentos);

    lista_ordena(listaSimilaridade, (cmp_fn)anonfn_sortKvpStrFloatDesc);

    // HashTable<string, int>
    HashTable *freqClasseDocumentos =
        ht_init((cpy_fn)strdup, (cpy_fn)intdup, (cmp_fn)strcmp, (free_fn)free,
                (free_fn)free);
    // Agrega a frequencia das classes dos k documentos mais similares

    for (int i = 0; i < k && i < lista_get_quantidade(listaSimilaridade); i++) {
        KeyValuePair *kvp = lista_get_elemento(listaSimilaridade, i);
        char *docNome = kvp_get_key(kvp);

        Documento *doc = ht_get(docsIdx, docNome);

        int *freq = ht_get(freqClasseDocumentos, doc_get_classe(doc));
        if (freq == NULL) {
            freq = malloc(sizeof(int));
            *freq = 1;

            ht_add(freqClasseDocumentos, doc_get_classe(doc), freq);

            free(freq);
        } else {
            (*freq)++;
        }
    }

    lista_dispose(listaSimilaridade);

    // Lista<KeyValuePair<string, int>>
    Lista *listaFreqClasseDocumentos = ht_toList(freqClasseDocumentos);

    ht_dispose(freqClasseDocumentos);

    lista_ordena(listaFreqClasseDocumentos, (cmp_fn)anonfn_sortKvpStrIntDesc);

    char *classe =
        strdup(kvp_get_key(lista_get_elemento(listaFreqClasseDocumentos, 0)));

    lista_dispose(listaFreqClasseDocumentos);

    return classe;
}
