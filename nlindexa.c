#include <stdio.h>
#include <stdlib.h>

#include "exception.h"

#include "indexador.h"
#include "repo_indices.h"

int main(int argc, char const *argv[]) {
    if (argc != 3)
        exception_throw("ArgumentException",
                        "Couldn't find command line argument for train txt "
                        "file or index bin file",
                        EXIT_FAILURE);

    FILE *train = fopen(argv[1], "r");

    Indice *idx = indexador_criaIndice(train);

    fclose(train);

    printf("Documentos indexados: %d\n", ht_get_qty(indice_get_documentos(idx)));
    printf("Palavras indexadas: %d\n", ht_get_qty(indice_get_palavras(idx)));

    repoidx_salvaIndice(idx, argv[2]);

    indice_dispose(idx);

    return EXIT_SUCCESS;
}
