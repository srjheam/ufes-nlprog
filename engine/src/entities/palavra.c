#include <stdlib.h>
#include <string.h>

#include "lista.h"

#include "palavra.h"

struct tPalavra {
    int id;
    char *conteudo;       // Palavra da palavra
    Lista *refDocumentos; // Lista<RefDocumento>
};

Palavra *palavra_init(int id, const char *conteudo,
                      const Lista *refDocumentos) {
    Palavra *palavra = malloc(sizeof *palavra);

    palavra->id = id;
    palavra->conteudo = strdup(conteudo);
    palavra->refDocumentos = lista_copia(refDocumentos, (cpy_fn)&refdoc_cpy);

    return palavra;
}

void palavra_dispose(Palavra *palavra) {
    free(palavra->conteudo);

    lista_dispose(palavra->refDocumentos, (free_fn)&refdoc_dispose);

    free(palavra);
}

int palavra_get_id(Palavra *palavra) { return palavra->id; }

char *palavra_get_conteudo(Palavra *palavra) { return palavra->conteudo; }

Lista *palavra_get_refDocumentos(Palavra *palavra) {
    return palavra->refDocumentos;
}
