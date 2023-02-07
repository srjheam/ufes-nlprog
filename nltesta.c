#include <stdio.h>
#include <stdlib.h>

#include "console.h"
#include "exception.h"
#include "indexador.h"
#include "indice.h"
#include "parser.h"
#include "repo_experimentos.h"
#include "repo_indices.h"
#include "testador.h"

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

    HashTable *matriz =
        testador_geraMatrizConfusao(idxBase, idxTest, k, &accuracy);

    char op = '\0';
    do {
        console_clear();
        printf("[t] - Exportar arquivo de texto tabulado\n"
               "[c] - Exportar arquivo padrão csv\n"
               "[q] - Sair\n"
               "Escolha uma opção: ");

        scanf("%c%*c", &op);

        console_clear();
        switch (op) {
        case 't':;
            printf("Salvando...\n");
            repoexp_salvaMatrizConfusaoTxt(argv[4], matriz, accuracy);
            printf("Salvo em %s\n", argv[4]);
            console_msg("");
            break;

        case 'c':;
            printf("Salvando...\n");
            repoexp_salvaMatrizConfusaoCsv(argv[4], matriz, accuracy);
            printf("Salvo em %s\n", argv[4]);
            console_msg("");
            break;

        case 'q':;
            printf("Até mais!\n");
            break;

        default:;
            console_msg("Opção inválida.");
            break;
        }
    } while (op != 'q');

    ht_dispose(matriz);

    indice_dispose(idxBase);
    indice_dispose(idxTest);

    return EXIT_SUCCESS;
}
