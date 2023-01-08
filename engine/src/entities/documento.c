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
    doc->refPalavras = lista_cpy(refPalavras);

    return doc;
}

void doc_dispose(Documento *doc) {
    free(doc->arquivo);
    free(doc->classe);

    lista_dispose(doc->refPalavras);

    free(doc);
}

int doc_get_id(Documento *doc) { return doc->id; }

char *doc_get_arquivo(Documento *doc) { return doc->arquivo; }

char *doc_get_classe(Documento *doc) { return doc->classe; }

Lista *doc_get_refPalavras(Documento *doc) { return doc->refPalavras; }
