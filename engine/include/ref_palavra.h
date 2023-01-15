#ifndef _REF_PALAVRA_
#define _REF_PALAVRA_

typedef struct tRefPalavra RefPalavra;

RefPalavra *refpalavra_init(const char *palavra, int freq);

void refpalavra_dispose(RefPalavra *refpalavra);

RefPalavra *refpalavra_cpy(RefPalavra *refpalavra);

char *refpalavra_get_palavra(RefPalavra *refpalavra);

int refpalavra_get_freq(RefPalavra *refpalavra);

#endif
