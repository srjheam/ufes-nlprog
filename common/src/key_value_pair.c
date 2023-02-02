#include <stdlib.h>

#include "key_value_pair.h"

#include "exception.h"

struct tKeyValuePair {
    void *chave;
    void *valor;

    cpy_fn cpyKey;
    cpy_fn cpyValue;

    free_fn disposeKey;
    free_fn disposeValue;
};

KeyValuePair *kvp_init(const void *chave, const void *valor, cpy_fn cpyKey,
                       cpy_fn cpyValue, free_fn disposeKey,
                       free_fn disposeValue) {
    KeyValuePair *par = malloc(sizeof *par);
    if (par == NULL)
        exception_throw_OutOfMemory("KeyValuePair malloc failed");

    par->chave = cpyKey(chave);
    par->valor = cpyValue(valor);

    par->cpyKey = cpyKey;
    par->cpyValue = cpyValue;

    par->disposeKey = disposeKey;
    par->disposeValue = disposeValue;

    return par;
}

void kvp_dispose(KeyValuePair *par) {
    par->disposeKey(par->chave);
    par->disposeValue(par->valor);

    free(par);
}

const void *kvp_get_key(const KeyValuePair *par) { return par->chave; }

void *kvp_get_value(const KeyValuePair *par) { return par->valor; }

void kvp_set_value(KeyValuePair *pair, const void *value) {
    pair->disposeValue(pair->valor);
    pair->valor = pair->cpyValue(value);
}

KeyValuePair *kvp_cpy(KeyValuePair *kvp) {
    KeyValuePair *cpy =
        kvp_init(kvp->chave, kvp->valor, kvp->cpyKey, kvp->cpyValue,
                 kvp->disposeKey, kvp->disposeValue);

    return cpy;
}
