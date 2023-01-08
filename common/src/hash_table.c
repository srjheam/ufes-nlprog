#include <stdbool.h>
#include <stdlib.h>

#include "hash_table.h"

#include "exception.h"
#include "key_value_pair.h"

struct tHashTable {
    Lista *pares; // Lista<KeyValuePair<void*, void*>>
    cmp_fn comparadorChaves;
    free_fn liberaChaves;
    free_fn liberaValores;
};

HashTable *ht_init(cmp_fn comparadorChaves, free_fn liberaChaves,
                               free_fn liberaValores) {
    HashTable *ht = malloc(sizeof *ht);
    if (ht == NULL)
        exception_throw_OutOfMemory("HashTable malloc failed");

    ht->pares = lista_init(ht->pares);
    ht->comparadorChaves = comparadorChaves;
    ht->liberaChaves = liberaChaves;
    ht->liberaValores = liberaValores;

    return ht;
}

void ht_dispose(HashTable *ht) {
    lista_dispose(ht->pares, (free_fn)&kvp_dispose);

    free(ht);
}

void **ht_index(HashTable *ht, const void *chave, cpy_fn copiaChave) {
    int n = lista_get_quantidade(ht->pares);

    void **val = NULL; // Ponteiro para o ponteiro do valor
    bool hasFound = false;

    int i;
    for (i = 0; i < n && !hasFound; i++) {
        KeyValuePair *curr =
            lista_get_elemento(ht->pares, i); // KeyValuePair<void*, void*>
        const void *currChave = kvp_get_key(curr);
        if (ht->comparadorChaves(currChave, chave) == 0) {
            val = kvp_ptr_value(curr);
            hasFound = true;
        }
    }

    if (!hasFound) {
        char *cpy_chave = (*copiaChave)(chave);
        if (cpy_chave == NULL)
            exception_throw_OutOfMemory(
                "HashTable internal GetValorHashTable chave copy failed");

        KeyValuePair *novo =
            kvp_init(cpy_chave, NULL, ht->liberaChaves, ht->liberaValores);

        lista_push(ht->pares, novo);

        val = kvp_ptr_value(novo);
    }

    return val;
}

Lista *ht_get_allkvps(HashTable *ht) { return ht->pares; }
