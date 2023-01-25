#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#include "documento.h"

struct tDocumento {
    char *arquivo;
    char *classe;
    HashTable *refPalavras; // HashTable<string, RefPalavra>
};

Documento *doc_init(const char *arquivo, const char *classe,
                    const HashTable *refPalavras) {
    Documento *doc = malloc(sizeof *doc);

    doc->arquivo = strdup(arquivo);
    doc->classe = strdup(classe);
    doc->refPalavras = ht_cpy(refPalavras);

    return doc;
}

void doc_dispose(Documento *doc) {
    free(doc->arquivo);
    free(doc->classe);

    ht_dispose(doc->refPalavras);

    free(doc);
}

char *doc_get_arquivo(Documento *doc) { return doc->arquivo; }

char *doc_get_classe(Documento *doc) { return doc->classe; }

HashTable *doc_get_refPalavras(Documento *doc) { return doc->refPalavras; }

Documento *doc_cpy(const Documento *doc) {
    return doc_init(doc->arquivo, doc->classe, doc->refPalavras);
}
