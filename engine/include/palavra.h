#ifndef _PALAVRA_
#define _PALAVRA_

#include "ref_documento.h"

typedef struct tPalavra Palavra;

Palavra *palavra_init(const char *conteudo, const Lista *refDocumentos);

void palavra_dispose(Palavra *palavra);

char *palavra_get_conteudo(Palavra *palavra);

Lista *palavra_get_refDocumentos(Palavra *palavra);

#endif
