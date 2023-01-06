#include <stdlib.h>

#include "lista.h"

#include "exception.h"

static const int ritmo_crescimento_lista = 5;

struct tLista {
    int capacidade;
    int qtd;
    void **arr;
};

Lista *lista_init() {
    Lista *lista = malloc(sizeof *lista);
    if (lista == NULL)
        exception_throw_OutOfMemory("Lista malloc failed");

    const int capacidade_inicial = 5;

    lista->capacidade = capacidade_inicial;
    lista->qtd = 0;
    lista->arr = malloc(lista->capacidade * __SIZEOF_POINTER__);
    if (lista->arr == NULL)
        exception_throw_OutOfMemory("Lista internal arr malloc failed");

    return lista;
}

void lista_dispose(Lista *lista, void (*liberaElem)(void *)) {
    for (int i = 0; i < lista->qtd; i++)
        (*liberaElem)(lista->arr[i]);

    free(lista->arr);

    free(lista);
}

static int (*gcmpval)(const void *, const void *) = NULL;

static inline int ptrvalcmp(const void **ptr1, const void **ptr2) {
    if (gcmpval == NULL)
        return 0;

    return gcmpval(*ptr1, *ptr2);
}

int lista_get_quantidade(const Lista *lista) { return lista->qtd; }

void *lista_get_elemento(Lista *lista, int i) {
    if (i < 0 || i >= lista->qtd)
        return NULL;

    return lista->arr[i];
}

void lista_push(Lista *lista, void *elem) {
    if (lista->qtd == lista->capacidade) {
        lista->capacidade += ritmo_crescimento_lista;
        lista->arr =
            realloc(lista->arr, lista->capacidade * __SIZEOF_POINTER__);
        if (lista->arr == NULL)
            exception_throw_OutOfMemory("Lista internal arr up realloc failed");
    }

    lista->arr[lista->qtd++] = elem;
}

void *lista_pop(Lista *lista) {
    if (lista->qtd < (lista->capacidade - 2 * ritmo_crescimento_lista)) {
        lista->capacidade -= ritmo_crescimento_lista;
        lista->arr =
            realloc(lista->arr, lista->capacidade * __SIZEOF_POINTER__);
        if (lista->arr)
            exception_throw_OutOfMemory(
                "Lista internal arr down realloc failed");
    }

    void *r = lista->arr[lista->qtd - 1];
    lista->arr[lista->qtd - 1] = NULL;
    lista->qtd--;

    return r;
}

int lista_encontra(Lista *lista, void *alvo,
                   int (*cmpElem)(const void *, const void *)) {
    int n = lista_get_quantidade(lista);
    for (int i = 0; i < n; i++)
        if (cmpElem(lista->arr[i], alvo) == 0)
            return i;

    return -1;
}

void lista_ordena(Lista *lista, int (*cmpElem)(const void *, const void *)) {
    gcmpval = cmpElem;
    qsort(lista->arr, lista->qtd, __SIZEOF_POINTER__, (cmp_fn)&ptrvalcmp);
    gcmpval = NULL;
}

Lista *lista_copia(const Lista *lista, void *(*cpyelem)(const void *)) {
    Lista *cpy = lista_init();

    int i;
    for (i = 0; i < lista->qtd; i++) {
        void *cpye = cpyelem(lista->arr[i]);
        if (cpye == NULL)
            exception_throw_OutOfMemory(
                "Lista internal lista_copia cpyelem failed");

        lista_push(cpy, cpye);
    }

    return cpy;
}
