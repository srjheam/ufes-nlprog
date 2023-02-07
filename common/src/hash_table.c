#include <stdbool.h>
#include <stdlib.h>

#include "exception.h"

#include "hash_table.h"

// Check this out:
// - https://codereview.stackexchange.com/a/254925

#define HT_INITIAL_SIZE 16
#define HT_GROWTH_FACTOR 2

struct tHashTable {
    KeyValuePair **items; // Array<Pointer<KeyValuePair<void, void>>>

    size_t size;   // how many items can be stored
    size_t scount; // how many items are stored
    size_t length; // how many items are there

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
    ht->scount = 0;
    ht->length = 0;

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

LinkedList *ht_keys(HashTable *ht) {
    // LinkedList<void>
    LinkedList *keys = ll_init((cpy_fn)ht->cpyKey, (free_fn)ht->disposeKey);

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ht_iter(ht, &saveptr)) != NULL) {
        ll_append(keys, ht->cpyKey(kvp_get_key(kvp)));
    }

    return keys;
}

static size_t ht_hash(const HashTable *ht, const void *chave) {
    return ht->hashKey(chave) & (ht->size - 1);
}

static void ht_resize(HashTable *ht, size_t nsize) {
    ht->size = nsize;

    KeyValuePair **nitems = calloc(ht->size, sizeof *ht->items);
    if (!nitems)
        exception_throw_OutOfMemory("HashTable grow failed");

    LinkedList *niterables = ll_init((cpy_fn)kvp_cpy, (free_fn)kvp_dispose);

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ll_iter(ht->iterables, &saveptr)) != NULL) {
        if (kvp_get_is_removed(kvp)) {
            kvp_dispose(kvp);
            continue;
        }

        size_t i = ht_hash(ht, kvp_get_key(kvp));

        while (nitems[i] != NULL)
            i = (i + 1) & (ht->size - 1);

        nitems[i] = kvp;

        ll_append(niterables, kvp);
    }

    free(ht->items);
    ll_i_dispose(ht->iterables);

    ht->items = nitems;
    ht->iterables = niterables;

    ht->scount = ht_get_length(ht);
}

static void ht_grow(HashTable *ht) {
    if (ht->scount >= 13 * (ht->size >> 4)) {
        if (ht_get_length(ht) >= 7 * (ht->size >> 4))
            ht_resize(ht, ht->size << 1);
        else
            ht_resize(ht, ht->size);
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

        ht->scount++;
        ht->length++;
    } else {
        if (kvp_get_is_removed(ht->items[i])) {
            ht->length++;
            kvp_set_removed(ht->items[i], false);
        }

        kvp_set_value(ht->items[i], value);
    }
}

static void ht_shrink(HashTable *ht) {
    if (ht->size <= HT_INITIAL_SIZE)
        return;

    if (ht_get_length(ht) <= 6 * (ht->size >> 5)) {
        ht_resize(ht, ht->size >> 1);
    }
}

const KeyValuePair *ht_remove(HashTable *ht, const void *chave) {
    ht_shrink(ht);

    size_t i = ht_hash(ht, chave);

    while (ht->items[i] != NULL &&
           !ht->cmpKey(kvp_get_key(ht->items[i]), chave) == 0)
        i = (i + 1) & (ht->size - 1);

    if (ht->items[i] == NULL) {
        return NULL;
    } else {
        kvp_set_removed(ht->items[i], true);

        ht->length--;

        return ht->items[i];
    }
}

void *ht_get(HashTable *ht, const void *chave) {
    size_t i = ht_hash(ht, chave);

    while (ht->items[i] != NULL &&
           !ht->cmpKey(kvp_get_key(ht->items[i]), chave) == 0)
        i = (i + 1) & (ht->size - 1);

    if (ht->items[i] == NULL || kvp_get_is_removed(ht->items[i]))
        return NULL;

    return kvp_get_value(ht->items[i]);
}

size_t ht_get_length(HashTable *ht) { return ht->length; }

Lista *ht_to_list(const HashTable *ht) {
    Lista *lista = lista_init((cpy_fn)kvp_cpy, (free_fn)kvp_dispose);

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ht_iter(ht, &saveptr)) != NULL)
        lista_push(lista, kvp_cpy(kvp));

    return lista;
}

HashTable *ht_cpy(const HashTable *ht) {
    HashTable *cpy = ht_init(ht->hashKey, ht->cpyKey, ht->cpyValue, ht->cmpKey,
                             ht->disposeKey, ht->disposeValue);

    cpy->size = ht->size;

    free(cpy->items);
    cpy->items = calloc(ht->size, sizeof *ht->items);
    if (!cpy->items)
        exception_throw_OutOfMemory("HashTable copy failed");

    void *saveptr = NULL;
    KeyValuePair *kvp = NULL;
    while ((kvp = ht_iter(ht, &saveptr)) != NULL) {
        size_t i = ht_hash(ht, kvp_get_key(kvp));

        while (cpy->items[i] != NULL)
            i = (i + 1) & (cpy->size - 1);

        KeyValuePair *kvpCpy = kvp_cpy(kvp);

        cpy->items[i] = kvpCpy;
        ll_append(cpy->iterables, kvpCpy);
        cpy->length++;
    }

    return cpy;
}

KeyValuePair *ht_iter(const HashTable *ht, void **saveptr) {
    KeyValuePair *v = NULL;

    while ((v = ll_iter(ht->iterables, saveptr)) != NULL &&
           kvp_get_is_removed(v))
        ;

    return v;
}
