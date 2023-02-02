#include <libgen.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "exception.h"
#include "extlib.h"
#include "indice.h"
#include "key_value_pair.h"
#include "palavra.h"
#include "ref_palavra.h"
#include "repo_noticias.h"

#include "relatorio.h"

static int cmp_qtd_documentos(KeyValuePair *x, KeyValuePair *y){
    return refdoc_get_freq(kvp_get_value(y)) - refdoc_get_freq(kvp_get_value(x));
}

static int cmp_qtd_classes(KeyValuePair *x, KeyValuePair *y){
    return (int *)kvp_get_value(y) - (int *)kvp_get_value(x);
}

static int cmp_len_doc(KeyValuePair *x, KeyValuePair *y){
    return ht_get_qty(doc_get_refPalavras(kvp_get_value(y))) 
         - ht_get_qty(doc_get_refPalavras(kvp_get_value(x)));
}

void relatorio_palavras(Indice *idx, char *query){
    //conferir se a palavra existe
    Palavra *palavra = NULL;
    if((palavra = ht_get(indice_get_palavras(idx), query)) == NULL){
        printf("Palavra inválida ou não presente em nenhum documento.\n");
        return;
    }
    HashTable *refs_docs = palavra_get_refDocumentos(palavra);

    //numero total de documentos
    printf("Quantidade de documentos em que a palavra está presente: %d\n\n",
            ht_get_qty(refs_docs));
    
    //os 10 em que ela mais aparece
    Lista *refdocs = ht_to_list(refs_docs);
    lista_ordena(refdocs, (cmp_fn)cmp_qtd_documentos);

    printf("10 documentos em que a palavra mais aparece:\n");
    for(int i=0;i<10;i++){
        printf("%d. %s\n", i, refdoc_get_documento(lista_get_elemento(refdocs, i)));
    }
    printf("\n");

    //frequencia da palavra por classe
    // Hashtable <string, int>
    HashTable *ht_classes = ht_init((cpy_fn)strdup, (cpy_fn)intdup, (cmp_fn)strcmp, (free_fn)free, (free_fn)free);
    int *saveptr = calloc(1, sizeof *saveptr);
    KeyValuePair *curr_refdoc = NULL;
    while ((curr_refdoc = ht_iter(refs_docs, saveptr)) != NULL) {
        char *doc_nome = refdoc_get_documento(kvp_get_value(curr_refdoc));
        char *curr_class = doc_get_classe(ht_get(indice_get_documentos(idx), doc_nome));
        
        if(ht_get(ht_classes, curr_class) == NULL)
        ht_add(ht_classes, curr_class, 0);

        int *freq_ptr = ht_get(ht_classes, curr_class);
        *(freq_ptr)+= 1;
    }
    free(saveptr);

    Lista *lista_classes = ht_to_list(ht_classes);
    lista_ordena(lista_classes, (cmp_fn)cmp_qtd_classes);

    printf("Frequência por classe:\n");
    for(int i = 0; i < lista_get_quantidade(lista_classes); i++){
        printf("Classe: %s - Frequência: %d\n",
                            (char *)kvp_get_key(lista_get_elemento(lista_classes, i)),
                            *(int *)kvp_get_value(lista_get_elemento(lista_classes, i)));
    }

}

void relatorio_documentos(Indice *idx){
    Lista *docs = ht_to_list(indice_get_documentos(idx));
    lista_ordena(docs, (cmp_fn)cmp_len_doc);

    printf("Documentos mais longos:\n");
    for(int i=0; i<lista_get_quantidade(docs); i++){
        printf("%d. Titulo: %s - Palavras: %d - Classe: %s", i
                                                       , doc_get_arquivo(kvp_get_value(lista_get_elemento(docs, i)))
                                                       , ht_get_qty(doc_get_refPalavras(kvp_get_value(lista_get_elemento(docs, i))))
                                                       , doc_get_classe(kvp_get_value(lista_get_elemento(docs, i))));
    }
    printf("\n");

    printf("Documentos mais curtos:\n");
    for(int i=lista_get_quantidade(docs); i>=0; i--){
        printf("%d. Titulo: %s - Palavras: %d - Classe: %s", i
                                                       , doc_get_arquivo(kvp_get_value(lista_get_elemento(docs, i)))
                                                       , ht_get_qty(doc_get_refPalavras(kvp_get_value(lista_get_elemento(docs, i))))
                                                       , doc_get_classe(kvp_get_value(lista_get_elemento(docs, i))));
    }
}