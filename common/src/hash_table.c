#include <stdbool.h>
#include <stdlib.h>

#include "exception.h"
#include "linked_list.h"

#include "hash_table.h"

// Check this out:
// - https://codereview.stackexchange.com/a/254925

#define HT_INITIAL_SIZE 16
#define HT_GROWTH_FACTOR 2

struct tHashTable {
    KeyValuePair **items; // Array<Pointer<KeyValuePair<void, void>>>

    size_t size; // how many items can be stored

    LinkedList *iterables; // LinkedList<KeyValuePair<void, void>>

    cpy_fn cpyValue;
    cpy_fn cpyKey;

    free_fn disposeKey;
    free_fn disposeValue;

    cmp_fn cmpKey;

    hash_fn hashKey;
};

HashTable *ht_init(hash_fn hashkey, cpy_fn cpyKey, cpy_fn cpyValue,
                   cmp_fn cmpKey, free_fn disposeKey, free_fn disposeValue) {
    HashTable *ht = malloc(sizeof *ht);
    if (ht == NULL)
        exception_throw_OutOfMemory("HashTable malloc failed");

    ht->items = calloc(HT_INITIAL_SIZE, sizeof *ht->items);

    ht->size = HT_INITIAL_SIZE;

    ht->iterables = ll_init((cpy_fn)kvp_cpy, (free_fn)kvp_dispose);

    ht->cpyValue = cpyValue;
    ht->cpyKey = cpyKey;

    ht->disposeKey = disposeKey;
    ht->disposeValue = disposeValue;

    ht->cmpKey = cmpKey;

    ht->hashKey = hashkey;

    return ht;
}

void ht_dispose(HashTable *ht) {
    ll_dispose(ht->iterables);

    free(ht->items);

    free(ht);
}

static size_t ht_hash(const HashTable *ht, const void *chave) {
    return ht->hashKey(chave) & (ht->size - 1);
}

static void ht_grow(HashTable *ht) {
    if (ht_get_length(ht) >= 13 * (ht->size >> 4)) {
        size_t nsize = ht->size * HT_GROWTH_FACTOR;
        KeyValuePair **nitems = calloc(nsize, sizeof *ht->items);
        if (!nitems)
            exception_throw_OutOfMemory("HashTable grow failed");

        void *saveptr = NULL;
        KeyValuePair *kvp = NULL;
        while ((kvp = ht_iter(ht, &saveptr)) != NULL) {
            size_t i = ht_hash(ht, kvp_get_key(kvp));

            while (nitems[i] != NULL)
                i = (i + 1) & (nsize - 1);

            nitems[i] = kvp;
        }

        free(ht->items);
        ht->items = nitems;
        ht->size = nsize;
    }
}

void ht_insert(HashTable *ht, const void *chave, const void *value) {
    ht_grow(ht);

    size_t i = ht_hash(ht, chave);

    while (ht->items[i] != NULL &&
           !ht->cmpKey(kvp_get_key(ht->items[i]), chave) == 0)
        i = (i + 1) & (ht->size - 1);

    if (ht->items[i] == NULL) {
        KeyValuePair *novo = kvp_init(chave, value, ht->cpyKey, ht->cpyValue,
                                      ht->disposeKey, ht->disposeValue);

        ht->items[i] = novo;

        ll_append(ht->iterables, novo);
    } else
        kvp_set_value(ht->items[i], value);
}

void *ht_get(HashTable *ht, const void *chave) {
    size_t i = ht_hash(ht, chave);

    while (ht->items[i] != NULL &&
           !ht->cmpKey(kvp_get_key(ht->items[i]), chave) == 0)
        i = (i + 1) & (ht->size - 1);

    return kvp_get_value(ht->items[i]);
}

size_t ht_get_length(HashTable *ht) { return ll_get_length(ht->iterables); }

Lista *ht_to_list(HashTable *ht) {
    Lista *lista = lista_init((cpy_fn)kvp_cpy, (free_fn)kvp_dispose);

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ht_iter(ht, &saveptr)) != NULL)
        lista_push(lista, kvp);

    return lista;
}

HashTable *ht_cpy(const HashTable *ht) {
    HashTable *cpy = ht_init(ht->hashKey, ht->cpyKey, ht->cpyValue, ht->cmpKey,
                             ht->disposeKey, ht->disposeValue);

    cpy->size = ht->size;

    cpy->iterables = ll_cpy(ht->iterables);

    cpy->items = calloc(ht->size, sizeof *ht->items);
    if (!cpy->items)
        exception_throw_OutOfMemory("HashTable copy failed");

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ht_iter(ht, &saveptr)) != NULL) {
        size_t i = ht_hash(ht, kvp_get_key(kvp));

        while (cpy->items[i] != NULL)
            i = (i + 1) & (cpy->size - 1);

        cpy->items[i] = kvp_cpy(kvp);
    }

    return cpy;
}

KeyValuePair *ht_iter(const HashTable *ht, void **saveptr) {
    return ll_iter(ht->iterables, saveptr);
}
