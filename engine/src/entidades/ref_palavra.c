#include <stdlib.h>
#include <string.h>

#include "ref_palavra.h"

struct tRefPalavra {
    char *palavra;
    int freqPalavra;
};

RefPalavra *refpalavra_init(const char *palavra, int freq) {
    RefPalavra *refpalavra = malloc(sizeof *refpalavra);

    refpalavra->palavra = strdup(palavra);
    refpalavra->freqPalavra = freq;

    return refpalavra;
}

void refpalavra_dispose(RefPalavra *refpalavra) {
    free(refpalavra->palavra);

    free(refpalavra);
}

RefPalavra *refpalavra_cpy(RefPalavra *refpalavra) {
    return refpalavra_init(refpalavra->palavra, refpalavra->freqPalavra);
}

char *refpalavra_get_palavra(RefPalavra *refpalavra) {
    return refpalavra->palavra;
}

int refpalavra_get_freq(RefPalavra *refpalavra) {
    return refpalavra->freqPalavra;
}

void refpalavra_incrementaFreqPor(RefPalavra *refpalavra, int value) {
    refpalavra->freqPalavra += value;
}
