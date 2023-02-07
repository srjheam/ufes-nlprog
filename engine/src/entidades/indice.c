#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "types.h"

#include "indice.h"

struct tIndice {
    HashTable *idxPalavras;   // HashTable<string, Palavra>
    HashTable *idxDocumentos; // HashTable<string, Documento>
};

Indice *indice_init(const HashTable *idxDocumentos,
                    const HashTable *idxPalavras) {
    Indice *idx = malloc(sizeof *idx);

    idx->idxPalavras = ht_cpy(idxPalavras);
    idx->idxDocumentos = ht_cpy(idxDocumentos);

    return idx;
}

void indice_dispose(Indice *idx) {
    ht_dispose(idx->idxDocumentos);
    ht_dispose(idx->idxPalavras);

    free(idx);
}

HashTable *indice_get_palavras(Indice *idx) { return idx->idxPalavras; }

HashTable *indice_get_documentos(Indice *idx) { return idx->idxDocumentos; }

const HashTable *indice_get_const_palavras(const Indice *idx) {
    return idx->idxPalavras;
}

const HashTable *indice_get_const_documentos(const Indice *idx) {
    return idx->idxDocumentos;
}

Indice *indice_cpy(const Indice *idx) {
    return indice_init(idx->idxDocumentos, idx->idxPalavras);
}
