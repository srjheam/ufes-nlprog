#include <stdbool.h>
#include <stdlib.h>

#include "hash_table.h"

#include "exception.h"

struct tHashTable {
    Lista *pares; // Lista<KeyValuePair<void*, void*>>
    cpy_fn copiaValor;
    cpy_fn copiaChave;
    cmp_fn comparadorChaves;
    free_fn liberaChaves;
    free_fn liberaValores;
};

HashTable *ht_init(cpy_fn copiaChave, cpy_fn copiaValor,
                   cmp_fn comparadorChaves, free_fn liberaChaves,
                   free_fn liberaValores) {
    HashTable *ht = malloc(sizeof *ht);
    if (ht == NULL)
        exception_throw_OutOfMemory("HashTable malloc failed");

    ht->pares = lista_init((cpy_fn)&kvp_cpy, (free_fn)&kvp_dispose);
    ht->copiaValor = copiaValor;
    ht->copiaChave = copiaChave;
    ht->comparadorChaves = comparadorChaves;
    ht->liberaChaves = liberaChaves;
    ht->liberaValores = liberaValores;

    return ht;
}

void ht_dispose(HashTable *ht) {
    lista_dispose(ht->pares);

    free(ht);
}

void ht_add(HashTable *ht, const void *chave, const void *value) {
    int n = lista_get_quantidade(ht->pares);

    int i;
    for (i = 0; i < n; i++) {
        KeyValuePair *curr =
            lista_get_elemento(ht->pares, i); // KeyValuePair<void*, void*>
        const void *currChave = kvp_get_key(curr);
        if (ht->comparadorChaves(currChave, chave) == 0) {
            kvp_set_value(curr, value);
            return;
        }
    }

    KeyValuePair *novo = kvp_init(chave, value, ht->copiaChave, ht->copiaValor,
                                  ht->liberaChaves, ht->liberaValores);

    lista_push(ht->pares, novo);
}

void *ht_get(HashTable *ht, const void *chave) {
    int n = lista_get_quantidade(ht->pares);

    int i;
    for (i = 0; i < n; i++) {
        KeyValuePair *curr =
            lista_get_elemento(ht->pares, i); // KeyValuePair<void*, void*>
        const void *currChave = kvp_get_key(curr);
        if (ht->comparadorChaves(currChave, chave) == 0) {
            return kvp_get_value(curr);
        }
    }

    return NULL;
}

int ht_get_qty(HashTable *ht) { return lista_get_quantidade(ht->pares); }

Lista *ht_to_list(HashTable *ht){ return lista_cpy(ht->pares); }

HashTable *ht_cpy(const HashTable *ht) {
    HashTable *cpy =
        ht_init(ht->copiaChave, ht->copiaValor, ht->comparadorChaves,
                ht->liberaChaves, ht->liberaValores);

    lista_dispose(cpy->pares);

    cpy->pares = lista_cpy(ht->pares);

    return cpy;
}

KeyValuePair *ht_iter(HashTable *ht, int *saveptr) {
    KeyValuePair *kvp = lista_get_elemento(ht->pares, *saveptr);

    *saveptr += 1;

    return kvp;
}
