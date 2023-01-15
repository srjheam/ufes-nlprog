#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

#include "documento.h"

struct tDocumento {
    int id;
    char *arquivo;
    char *classe;
    HashTable *htPalavras; // HashTable<int, RefPalavra>
};

Documento *doc_init(int id, const char *arquivo, const char *classe,
                    const HashTable *htPalavras) {
    Documento *doc = malloc(sizeof *doc);

    doc->id = id;
    doc->arquivo = strdup(arquivo);
    doc->classe = strdup(classe);
    doc->htPalavras = ht_cpy(htPalavras);

    return doc;
}

void doc_dispose(Documento *doc) {
    free(doc->arquivo);
    free(doc->classe);

    ht_dispose(doc->htPalavras);

    free(doc);
}

int doc_get_id(Documento *doc) { return doc->id; }

char *doc_get_arquivo(Documento *doc) { return doc->arquivo; }

char *doc_get_classe(Documento *doc) { return doc->classe; }

HashTable *doc_get_htPalavras(Documento *doc) { return doc->htPalavras; }
