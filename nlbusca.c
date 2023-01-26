#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exception.h"
#include "hash_table.h"
#include "parser.h"
#include "repo_indices.h"

int main(int argc, char const *argv[]) {
    if (argc != 3)
        exception_throw(
            "ArgumentException",
            "Couldn't find command line argument for binary file or K-onstant",
            EXIT_FAILURE);

    int *k = parseInt(argv[2]);

    Indice *idx = repoidx_carregaIndice(argv[1]);

    // TODO: Fazer o menu para selecionar entre as funções do programa (buscar, classificar, etc.)

    free(k);
    indice_dispose(idx);

    return EXIT_SUCCESS;
}
