#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classificador.h"
#include "exception.h"
#include "hash_table.h"
#include "parser.h"
#include "relatorio.h"
#include "repo_indices.h"
#include "repo_noticias.h"

int main(int argc, char const *argv[]) {
    if (argc != 3)
        exception_throw(
            "ArgumentException",
            "Couldn't find command line argument for binary file or K-onstant",
            EXIT_FAILURE);

    int k = parseInt(argv[2]);

    Indice *idx = repoidx_carregaIndice(argv[1]);

    printf("[q] - Buscar notícias\n"
           "[c] - Classificar notícias\n"
           "[f] - Relatório de palavra\n"
           "[l] - Relatório de documentos\n"
           "Escolha uma opção: ");

    char op = '\0';
    scanf("%c%*c", &op);

    system("clear");
    switch (op) {
    case 'q':

        break;

    case 'c':
        printf("Informe o texto da notícia:\n");

        Documento *inNoticia =
            reponoticias_carregaDocumento(stdin, "input", "tbd");

        char *classe = classificador_classificaDocumento(inNoticia, idx, k);

        printf("\n\nA notícia pertence à classe '%s'\n", classe);

        free(classe);
        doc_dispose(inNoticia);
        break;

    case 'f':;
        printf("Informe uma palavra: \n");

        char *query = NULL;
        size_t len = 0;
        int strlen = 0;
        if ((strlen = getline(&query, &len, stdin)) == -1)
            exception_throw_failure("Erro ao ler a palavra.\n");

        if (query[strlen - 1] == '\n')
            query[strlen - 1] = '\0';

        relatorio_palavras(idx, query);

        free(query);
        break;

    case 'l':
        relatorio_documentos(idx);
        break;

    default:
        indice_dispose(idx);

        exception_throw_failure("Opção inválida. Saindo do programa...\n");
        break;
    }

    indice_dispose(idx);

    return EXIT_SUCCESS;
}
