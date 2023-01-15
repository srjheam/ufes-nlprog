#ifndef _REF_DOC_
#define _REF_DOC_

typedef struct tRefDocumento RefDocumento;

/**
 * @brief
 *
 * @param documento Nome do documento (34234324poc)
 * @param freq
 * @param tfIdf
 * @return RefDocumento*
 */
RefDocumento *refdoc_init(const char *documento, int freq, float tfIdf);

void refdoc_dispose(RefDocumento *refdoc);

RefDocumento *refdoc_cpy(RefDocumento *refdoc);

char *refdoc_get_documento(RefDocumento *refdoc);

int refdoc_get_freq(RefDocumento *refdoc);

float refdoc_get_tdIdf(RefDocumento *refdoc);

#endif
