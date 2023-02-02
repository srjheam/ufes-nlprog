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

    Indice *idx = indexador_criaIndice(argv[1]);

    printf("Documentos indexados: %d\n", ht_get_length(indice_get_documentos(idx)));
    printf("Palavras indexadas: %d\n", ht_get_length(indice_get_palavras(idx)));

    repoidx_salvaIndice(idx, argv[2]);

    indice_dispose(idx);

    return EXIT_SUCCESS;
}
