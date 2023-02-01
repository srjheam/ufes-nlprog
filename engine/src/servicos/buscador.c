#include <libgen.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "exception.h"
#include "key_value_pair.h"
#include "palavra.h"
#include "ref_palavra.h"
#include "repo_noticias.h"
#include "indice.h"

#include "buscador.h"

Lista *BuscaNoticias(char *query, Indice *idx){
    query[strlen(query)] = '\0';

    //HashTable <string, float>
    HashTable *documentos = ht_init((cpy_fn)strdup, (cpy_fn)lib_intdup,
                            (cmp_fn)strcmp, (free_fn)free, (free_fn)free);

    char *saveptr = NULL, *token = NULL;
    int i;
    for (i = 1, token = query;; i++, token = NULL) {
        token = strtok_r(token, " ", &saveptr);
        if (token == NULL)
            break;

        HashTable *refs_doc = palavra_get_refDocumentos(ht_get(indice_get_palavras(idx), token));

        int *saveptr = calloc(1, sizeof *saveptr);
        KeyValuePair *curr_refdoc = NULL;
        while ((curr_refdoc = ht_iter(refs_doc, saveptr)) != NULL) {
            RefDocumento *refdoc = kvp_get_value(curr_refdoc);
            char *titulo = refdoc_get_documento(refdoc);

            if(ht_get(documentos, titulo) == NULL)
                ht_add(documentos, titulo, 0);

            float *tfidf_ptr = ht_get(documentos, titulo);
            *tfidf_ptr += refdoc_get_tdIdf(refdoc);
        }
        free(saveptr);
    }

    Lista *lista_noticias = ht_to_list(documentos);

    return lista_noticias;

}