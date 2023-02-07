#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buscador.h"
#include "classificador.h"
#include "console.h"
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

    Indice *idxCarregado = repoidx_carregaIndice(argv[1]);

    const Indice *idx = idxCarregado;

    char op = '\0';
    do {
        console_clear();
        printf("[s] - Buscar notícias\n"
               "[c] - Classificar notícias\n"
               "[f] - Relatório de palavra\n"
               "[l] - Relatório de documentos\n"
               "[q] - Sair\n"
               "Escolha uma opção: ");

        scanf("%c%*c", &op);

        console_clear();
        switch (op) {
        case 's':
            printf("Informe sua busca: \n");

            char *query = NULL;
            size_t len = 0;
            int strlen = 0;
            if ((strlen = getline(&query, &len, stdin)) == -1)
                exception_throw_failure("Erro ao ler a palavra.\n");

            if (query[strlen - 1] == '\n')
                query[strlen - 1] = '\0';

            HashTable *idxPalCpy = ht_cpy(indice_get_const_palavras(idx));

            Lista *relevantes = buscador_buscaNoticias(query, idxPalCpy);

            ht_dispose(idxPalCpy);

            int n = lista_get_quantidade(relevantes);
            if (n == 0) {
                printf("Nenhuma notícia encontrada.\n");
            } else {
                printf("\n\nAs 10 notícias mais relevantes:\n");
                for (int i = 0; i < 10 && i < n; i++) {
                    KeyValuePair *kvp = lista_get_elemento(relevantes, i);
                    printf("%d. %s (%.2f)\n", i, (char *)kvp_get_key(kvp),
                           *(float *)kvp_get_value(kvp));
                }
            }

            free(query);
            lista_dispose(relevantes);

            console_msg("");

            break;

        case 'c':
            printf("Informe o texto da notícia:\n");

            Documento *inNoticia =
                reponoticias_carregaDocumento(stdin, "input", "tbd");

            HashTable *docsIdxCpy = ht_cpy(indice_get_const_documentos(idx));

            char *classe =
                classificador_classificaDocumento(inNoticia, docsIdxCpy, k);

            ht_dispose(docsIdxCpy);

            printf("\n\nA notícia pertence à classe '%s'\n", classe);

            free(classe);
            doc_dispose(inNoticia);

            console_msg("");

            break;

        case 'f':;
            printf("Informe uma palavra: \n");

            query = NULL;
            len = 0;
            strlen = 0;
            if ((strlen = getline(&query, &len, stdin)) == -1)
                exception_throw_failure("Erro ao ler a palavra.\n");

            if (query[strlen - 1] == '\n')
                query[strlen - 1] = '\0';

            Indice *cpy = indice_cpy(idx);

            relatorio_palavras(cpy, query);

            indice_dispose(cpy);

            free(query);

            console_msg("");

            break;

        case 'l':
            relatorio_documentos(idx);

            console_msg("");

            break;

        case 'q':
            printf("Até mais!\n");
            break;

        default:
            console_msg("Opção inválida.");
            break;
        }
    } while (op != 'q');

    indice_dispose(idxCarregado);

    return EXIT_SUCCESS;
}
