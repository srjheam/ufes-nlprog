#ifndef _DOC_
#define _DOC_

#include "ref_palavra.h"

typedef struct tDocumento Documento;

Documento *doc_init(int id, const char *arquivo, const char *classe,
                    const Lista *refPalavras);

void doc_dispose(Documento *doc);

int doc_get_id(Documento *doc);

char *doc_get_arquivo(Documento *doc);

char *doc_get_classe(Documento *doc);

Lista *doc_get_refPalavras(Documento *doc);

#endif