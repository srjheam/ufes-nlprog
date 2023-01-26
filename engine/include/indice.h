#ifndef _INDICE_
#define _INDICE_

#include "hash_table.h"

typedef struct tIndice Indice;

Indice *indice_init(const HashTable* idxDocumentos, const HashTable* idxPalavras);

void indice_dispose(Indice *idx);

HashTable *indice_get_palavras(Indice *idx);

HashTable *indice_get_documentos(Indice *idx);

#endif
