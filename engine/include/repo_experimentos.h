#ifndef _REPO_EXPERIMENTOS_
#define _REPO_EXPERIMENTOS_

#include "hash_table.h"

void repoexp_salvaMatrizConfusaoCsv(const char *filename, HashTable *matriz,
                                    float accuracy);

#endif // _REPO_EXPERIMENTOS_
