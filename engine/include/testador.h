#ifndef _TESTADOR_
#define _TESTADOR_

#include "hash_table.h"
#include "indice.h"

HashTable *testador_geraMatrizConfusao(Indice *idxBase, Indice *idxTest, int k, float *accuracy);

#endif // _TESTADOR_
