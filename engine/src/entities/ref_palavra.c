#include <stdlib.h>

#include "ref_palavra.h"

struct tRefPalavra {
    int idPalavra;
    int freqPalavra;
};

RefPalavra *refpalavra_init(int id, int freq) {
    RefPalavra *refpalavra = malloc(sizeof *refpalavra);

    refpalavra->idPalavra = id;
    refpalavra->freqPalavra = freq;

    return refpalavra;
}

void refpalavra_dispose(RefPalavra *refpalavra) { free(refpalavra); }

RefPalavra *refpalavra_cpy(RefPalavra *refpalavra) {
    return refpalavra_init(refpalavra->idPalavra, refpalavra->freqPalavra);
}

int refpalavra_get_id(RefPalavra *refpalavra) { return refpalavra->idPalavra; }

int refpalavra_get_freq(RefPalavra *refpalavra) {
    return refpalavra->freqPalavra;
}
