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
    // KeyValuePair<string, Palavra>
    HashTable *idxPalavras = ht_init((cpy_fn)strdup, (cmp_fn)strcmp,
                                     (free_fn)free, (free_fn)palavra_dispose);

    int *saveptr = NULL;
    KeyValuePair *curr = NULL;
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        // KeyValuePair<string, Documento>
        //Documento *doc = kvp_get_value(curr);

        /* Palavra *pal = ht_get(idxPalavras, );
        if (pal == NULL)
            ht_add(idxPalavras, ) */
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
