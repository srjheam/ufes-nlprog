#include <stdlib.h>

#include "ref_documento.h"

struct tRefDocumento {
    char *documento;
    int freqPalavra;
    float tfIdf;
};

RefDocumento *refdoc_init(const char *documento, int freq, float tfIdf) {
    RefDocumento *refdoc = malloc(sizeof *refdoc);

    refdoc->documento = documento;
    refdoc->freqPalavra = freq;
    refdoc->tfIdf = tfIdf;

    return refdoc;
}

void refdoc_dispose(RefDocumento *refdoc) {
    free(refdoc->documento);

    free(refdoc);
}

RefDocumento *refdoc_cpy(RefDocumento *refdoc) {
    return refdoc_init(refdoc->documento, refdoc->freqPalavra, refdoc->tfIdf);
}

char *refdoc_get_documento(RefDocumento *refdoc) { return refdoc->documento; }

int refdoc_get_freq(RefDocumento *refdoc) { return refdoc->freqPalavra; }

float refdoc_get_tdIdf(RefDocumento *refdoc) { return refdoc->tfIdf; }
