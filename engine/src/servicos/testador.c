#include <string.h>

#include "classificador.h"
#include "documento.h"
#include "extlib.h"

#include "testador.h"

HashTable *testador_geraMatrizConfusao(Indice *idxBase, Indice *idxTest, int k,
                                       float *accuracy) {
    HashTable *trueClasses =
        ht_init((hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)ht_cpy,
                (cmp_fn)strcmp, (free_fn)free, (free_fn)ht_dispose);

    *accuracy = 0;

    int totalPredictions = 0;
    int totalCorrectPredictions = 0;

    KeyValuePair *curr_doc = NULL;
    void *saveptr = NULL;
    while ((curr_doc = ht_iter(indice_get_documentos(idxTest), &saveptr)) !=
           NULL) {
        Documento *doc = kvp_get_value(curr_doc);

        // HashTable <string, Documento>
        HashTable *idxBaseNDocs = ht_cpy(indice_get_documentos(idxBase));

        char *predictCls =
            classificador_classificaDocumento(doc, idxBaseNDocs, k);

        ht_dispose(idxBaseNDocs);

        char *trueCls = doc_get_classe(doc);

        HashTable *predictClasses = ht_get(trueClasses, trueCls);

        if (predictClasses == NULL) {
            // HashTable<string, int>
            predictClasses =
                ht_init((hash_fn)hashStr, (cpy_fn)strdup, (cpy_fn)intdup,
                        (cmp_fn)strcmp, (free_fn)free, (free_fn)free);

            ht_insert(trueClasses, trueCls, predictClasses);

            ht_dispose(predictClasses);

            predictClasses = ht_get(trueClasses, trueCls);
        }

        int *predictClassCount = ht_get(predictClasses, predictCls);
        if (predictClassCount == NULL) {
            int c = 1;

            ht_insert(predictClasses, predictCls, &c);
        } else
            (*predictClassCount)++;

        if (strcmp(trueCls, predictCls) == 0) {
            totalCorrectPredictions++;
        }

        totalPredictions++;

        free(predictCls);
    }

    *accuracy = (totalCorrectPredictions / (float)totalPredictions) * 100;

    return trueClasses;
}
