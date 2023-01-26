#ifndef _DOC_
#define _DOC_

#include "hash_table.h"
#include "ref_palavra.h"

typedef struct tDocumento Documento;

Documento *doc_init(const char *arquivo, const char *classe,
                    const HashTable *refPalavras);

void doc_dispose(Documento *doc);

char *doc_get_arquivo(Documento *doc);

char *doc_get_classe(Documento *doc);

HashTable *doc_get_refPalavras(Documento *doc);

Documento *doc_cpy(const Documento *doc);

#endif