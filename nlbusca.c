#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classificador.h"
#include "exception.h"
#include "hash_table.h"
#include "parser.h"
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

    system("clear");

    char op = '\0';
    scanf("%c%*c", op);
    switch (op) {
    case 'q':

        break;

    case 'c':
        printf("Informe o texto da notícia:\n");

        char *line = NULL;
        size_t len = 0;
        ssize_t read = getline(&line, &len, stdin);

        if (read == -1)
            exception_throw_failure(
                "Erro ao ler texto notícia. Saindo do programa...\n");

        Documento *inNoticia =
            reponoticias_carregaDocumento(stdin, "input", "tbd");

        char *classe = classificador_classificaDocumento(inNoticia, idx, k);

        printf("\n\nA notícia pertence à classe '%s'\n", classe);

        free(classe);
        doc_dispose(inNoticia);
        free(line);
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
