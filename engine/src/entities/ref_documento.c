#include <stdlib.h>

#include "ref_documento.h"

struct tRefDocumento {
    int idDocumento;
    int freqPalavra;
    float tfIdf;
};

RefDocumento *refdoc_init(int id, int freq, float tfIdf) {
    RefDocumento *refdoc = malloc(sizeof *refdoc);

    refdoc->idDocumento = id;
    refdoc->freqPalavra = freq;
    refdoc->tfIdf = tfIdf;

    return refdoc;
}

void refdoc_dispose(RefDocumento *refdoc) { free(refdoc); }

RefDocumento *refdoc_cpy(RefDocumento *refdoc) {
    return refdoc_init(refdoc->idDocumento, refdoc->freqPalavra, refdoc->tfIdf);
}
