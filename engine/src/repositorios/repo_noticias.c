#include <stdlib.h>
#include <string.h>

#include "documento.h"

#include "repo_noticias.h"

Documento *reponoticias_carregaDocumento(FILE *noticia, char *nome,
                                         char *classe) {
    char *buffer = NULL;
    size_t len = 0;
    if (getline(&buffer, &len, noticia) == -1) {
        free(buffer);
        return NULL;
    }

    // HashTable<string, RefPalavra>
    HashTable *refPalavras =
        ht_init((cpy_fn)strdup, (cpy_fn)refpalavra_cpy, (cmp_fn)strcmp, (free_fn)free,
                (free_fn)refpalavra_dispose);

    char *saveptr = NULL, *token = NULL;
    int i;
    for (i = 1, token = buffer;; i++, token = NULL) {
        token = strtok_r(token, " ", &saveptr);
        if (token == NULL)
            break;

        RefPalavra *ref = ht_get(refPalavras, token);
        if (ref == NULL)
            ht_add(refPalavras, token, refpalavra_init(token, 1));
        else
            refpalavra_incrementaFreqPor(ref, 1);
    }
    free(buffer);

    Documento *doc = doc_init(nome, classe, refPalavras);

    ht_dispose(refPalavras);

    return doc;
}
