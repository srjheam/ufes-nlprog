#include <stdlib.h>
#include <string.h>

#include "lista.h"

#include "documento.h"

struct tDocumento {
    int id;
    char *arquivo;
    char *classe;
    Lista *refPalavras; // Lista<RefPalavra>
};

Documento *doc_init(int id, const char *arquivo, const char *classe,
                    const Lista *refPalavras) {
    Documento *doc = malloc(sizeof *doc);

    doc->id = id;
    doc->arquivo = strdup(arquivo);
    doc->classe = strdup(classe);
    doc->refPalavras = lista_copia(refPalavras, (cpy_fn)&refpalavra_cpy);

    return doc;
}

void doc_dispose(Documento *doc) {
    free(doc->arquivo);
    free(doc->classe);

    lista_dispose(doc->refPalavras, (free_fn)&refpalavra_dispose);

    free(doc);
}
