#ifndef _REF_DOC_
#define _REF_DOC_

typedef struct tRefDocumento RefDocumento;

RefDocumento *refdoc_init(int id, int freq, float tfIdf);

void refdoc_dispose(RefDocumento *refdoc);

RefDocumento *refdoc_cpy(RefDocumento *refdoc);

int refdoc_get_id(RefDocumento *refdoc);

int refdoc_get_freq(RefDocumento *refdoc);

float refdoc_get_tdIdf(RefDocumento *refdoc);

#endif
