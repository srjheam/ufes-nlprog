#include <stdlib.h>

#include "key_value_pair.h"

#include "exception.h"

struct tKeyValuePair {
    void *chave;
    void *valor;
    cpy_fn copiaChave;
    cpy_fn copiaValor;
    free_fn liberaChave;
    free_fn liberaValor;
};

KeyValuePair *kvp_init(const void *chave, const void *valor, cpy_fn copiaChave,
                       cpy_fn copiaValor, free_fn liberaChave,
                       free_fn liberaValor) {
    KeyValuePair *par = malloc(sizeof *par);
    if (par == NULL)
        exception_throw_OutOfMemory("KeyValuePair malloc failed");

    par->chave = copiaChave(chave);
    par->valor = copiaValor(valor);
    par->copiaChave = copiaChave;
    par->copiaValor = copiaValor;
    par->liberaChave = liberaChave;
    par->liberaValor = liberaValor;

    return par;
}

void kvp_dispose(KeyValuePair *par) {
    par->liberaChave(par->chave);
    par->liberaValor(par->valor);

    free(par);
}

const void *kvp_get_key(const KeyValuePair *par) { return par->chave; }

void *kvp_get_value(const KeyValuePair *par) { return par->valor; }

void **kvp_ptr_value(KeyValuePair *pair) { return &(pair->valor); }

KeyValuePair *kvp_cpy(KeyValuePair *kvp) {
    KeyValuePair *cpy =
        kvp_init(kvp->chave, kvp->valor, kvp->copiaChave, kvp->copiaValor,
                 kvp->liberaChave, kvp->liberaValor);

    return cpy;
}
