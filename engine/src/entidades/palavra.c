#include <stdlib.h>
#include <string.h>

#include "lista.h"

#include "palavra.h"

struct tPalavra {
    char *conteudo;       // Palavra da palavra
    Lista *refDocumentos; // Lista<RefDocumento>
};

Palavra *palavra_init(const char *conteudo,
                      const Lista *refDocumentos) {
    Palavra *palavra = malloc(sizeof *palavra);

    palavra->conteudo = strdup(conteudo);
    palavra->refDocumentos = lista_cpy(refDocumentos);

    return palavra;
}

void palavra_dispose(Palavra *palavra) {
    free(palavra->conteudo);

    lista_dispose(palavra->refDocumentos);

    free(palavra);
}

char *palavra_get_conteudo(Palavra *palavra) { return palavra->conteudo; }

Lista *palavra_get_refDocumentos(Palavra *palavra) {
    return palavra->refDocumentos;
}
