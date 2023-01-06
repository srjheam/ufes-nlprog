#ifndef _PALAVRA_
#define _PALAVRA_

#include "ref_documento.h"

typedef struct tPalavra Palavra;

Palavra *palavra_init(int id, const char *conteudo, const Lista *refDocumentos);

void palavra_dispose(Palavra *palavra);

#endif
