#ifndef _PALAVRA_
#define _PALAVRA_

#include "ref_documento.h"

typedef struct tPalavra Palavra;

Palavra *palavra_init(int id, const char *conteudo, const Lista *refDocumentos);

void palavra_dispose(Palavra *palavra);

int palavra_get_id(Palavra *palavra);

char *palavra_get_conteudo(Palavra *palavra);

Lista *palavra_get_refDocumentos(Palavra *palavra);

#endif
