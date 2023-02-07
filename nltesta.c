#include <stdio.h>
#include <stdlib.h>

#include "exception.h"
#include "indice.h"
#include "repo_experimentos.h"
#include "repo_indices.h"
#include "testador.h"
#include "parser.h"
#include "indexador.h"

int main(int argc, char const *argv[]) {
    if (argc != 5)
        exception_throw("ArgumentException",
                        "Couldn't find command line argument for binary file, "
                        "test file, K-onstant or output file",
                        EXIT_FAILURE);

    int k = parseInt(argv[3]);

    Indice *idxBase = repoidx_carregaIndice(argv[1]);

    Indice *idxTest = indexador_criaIndice(argv[2]);

    float accuracy = 0;

    HashTable *matriz = testador_geraMatrizConfusao(idxBase, idxTest, k, &accuracy);

    repoexp_salvaMatrizConfusaoTxt(argv[4], matriz, accuracy);

    ht_dispose(matriz);

    indice_dispose(idxBase);
    indice_dispose(idxTest);

    return EXIT_SUCCESS;
}
