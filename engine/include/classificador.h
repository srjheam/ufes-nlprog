#ifndef _CLASSIFICADOR_
#define _CLASSIFICADOR_

#include "documento.h"
#include "indice.h"

char *classificador_classificaDocumento(Documento *documento,
                                        HashTable *docsIdx, int k);

#endif
