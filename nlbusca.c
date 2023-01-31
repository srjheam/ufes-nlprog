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

    printf("[q] - Buscar notícias\n"
           "[c] - Classificar notícias\n"
           "[f] - Relatório de palavra\n"
           "[l] - Relatório de documentos\n"
           "Escolha uma opção: ");

    system("clear");

    char op = '\0';
    scanf("%c%*c", op);
    switch (op) {
    case 'q':
        
        break;

    case 'c':
        
        break;

    case 'f':

        break;

    case 'l':

        break;

    default:
        free(k);
        indice_dispose(idx);

        exception_throw_failure("Opção inválida. Saindo do programa...\n");
        break;
    }

    free(k);
    indice_dispose(idx);

    return EXIT_SUCCESS;
}
