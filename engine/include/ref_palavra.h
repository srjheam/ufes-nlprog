#ifndef _REF_PALAVRA_
#define _REF_PALAVRA_

typedef struct tRefPalavra RefPalavra;

RefPalavra *refpalavra_init(int id, int freq);

void refpalavra_dispose(RefPalavra *refpalavra);

RefPalavra *refpalavra_cpy(RefPalavra *refpalavra);

#endif