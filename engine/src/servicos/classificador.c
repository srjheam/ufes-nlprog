#include <stdlib.h>
#include <string.h>

#include "hash_table.h"
#include "indexador.h"

#include "classificador.h"

Documento *classificador_classificaDocumento(Documento *doc, Indice* idx) {
    HashTable *docs = indice_get_documentos(idx);

    ht_add(docs, doc_get_arquivo(doc), doc);

    HashTable *idxPalavras = indexador_criaIdxPalavras(docs);

    HashTable *similaridadeDocumenotos = ht_init((cpy_fn)strdup, (cpy_fn)lib_intdup, (cmp_fn)strcmp, (free_fn)free, (free_fn)free);

    ht_dispose(idxPalavras);
    ht_dispose(similaridadeDocumenotos);

    return doc;
}
