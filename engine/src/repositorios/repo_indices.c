#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "exception.h"
#include "palavra.h"
#include "ref_documento.h"
#include "ref_palavra.h"

#include "repo_indices.h"

static void repoidx_salvaIndiceDocumentos(HashTable *idxDocumentos, FILE *fbin) {
    // Salva a quantidade de documentos
    int idxDocumentosQty = ht_get_qty(idxDocumentos);
    if (fwrite(&idxDocumentosQty, sizeof idxDocumentosQty, 1, fbin) != 1) {
        exception_throw_failure(
            "Erro ao salvar idxDocumentosQty do indice no bin - em "
            "engine/repositorios/repo_indices.repoidx_salvaIndiceDocumentos");
    }

    // KVP<string, Documento>
    KeyValuePair *curr = NULL;
    int *saveptr = calloc(1, sizeof *saveptr);
    while ((curr = ht_iter(idxDocumentos, saveptr)) != NULL) {
        Documento *doc = kvp_get_value(curr);

        // Salva nome de um documento com final '\0'
        char *nome = doc_get_arquivo(doc);
        size_t nlen = strlen(nome) + 1;
        if (fwrite(nome, nlen, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao salvar nome do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_salvaIndiceDocumentos");
        }

        // Salva classe de um documento com final '\0'
        char *classe = doc_get_classe(doc);
        nlen = strlen(classe) + 1;
        if (fwrite(classe, nlen, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao salvar classe do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_salvaIndiceDocumentos");
        }

        HashTable *refPalavra = doc_get_refPalavras(doc);

        // Salva a quantidade de RefPalavra
        int refPalavraQty = ht_get_qty(refPalavra);
        if (fwrite(&refPalavraQty, sizeof refPalavraQty, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao salvar refPalavraQty do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_salvaIndiceDocumentos");
        }

        // KVP<string, RefPalavra>
        KeyValuePair *curr = NULL;
        int *saveptr = calloc(1, sizeof *saveptr);
        while ((curr = ht_iter(refPalavra, saveptr)) != NULL) {
            RefPalavra *ref = kvp_get_value(curr);

            // Salva palavra com final '\0'
            char *palavra = refpalavra_get_palavra(ref);
            size_t nlen = strlen(palavra) + 1;
            if (fwrite(palavra, nlen, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao salvar palavra do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_salvaIndiceDocumentos");
            }

            // Salva frequencia de um refpalavra
            int freq = refpalavra_get_freq(ref);
            if (fwrite(&freq, sizeof freq, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao salvar frequencia do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_salvaIndiceDocumentos");
            }
        }
        free(saveptr);
    }
    free(saveptr);
}

static void repoidx_salvaIndicePalavras(HashTable *idxPalavras, FILE *fbin) {
    // Salva a quantidade de palavras
    int idxPalavrasQty = ht_get_qty(idxPalavras);
    if (fwrite(&idxPalavrasQty, sizeof idxPalavrasQty, 1, fbin) != 1) {
        exception_throw_failure(
            "Erro ao salvar idxPalavrasQty do indice no bin - em "
            "engine/repositorios/repo_indices.repoidx_salvaIndicePalavras");
    }

    // KVP<string, Palavra>
    KeyValuePair *curr = NULL;
    int *saveptr = calloc(1, sizeof *saveptr);
    while ((curr = ht_iter(idxPalavras, saveptr)) != NULL) {
        Palavra *pal = kvp_get_value(curr);

        // Salva palavra com final '\0'
        char *palavra = palavra_get_conteudo(pal);
        size_t nlen = strlen(palavra) + 1;
        if (fwrite(palavra, nlen, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao salvar palavra do indice no bin - em "
                "engine/repositorios/repo_indices.repoidx_salvaIndicePalavras");
        }

        HashTable *refDocumento = palavra_get_refDocumentos(pal);

        // Salva a quantidade de RefPalavra
        int refDocumentoQty = ht_get_qty(refDocumento);
        if (fwrite(&refDocumentoQty, sizeof refDocumentoQty, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao salvar refDocumentoQty do indice no bin - em "
                "engine/repositorios/repo_indices.repoidx_salvaIndicePalavras");
        }

        // KVP<string, RefPalavra>
        KeyValuePair *curr = NULL;
        int *saveptr = calloc(1, sizeof *saveptr);
        while ((curr = ht_iter(refDocumento, saveptr)) != NULL) {
            RefDocumento *ref = kvp_get_value(curr);

            // Salva nome refdocumento com final '\0'
            char *doc = refdoc_get_documento(ref);
            size_t nlen = strlen(doc) + 1;
            if (fwrite(doc, nlen, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao salvar palavra do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_salvaIndicePalavras");
            }

            // Salva frequencia de um refdocumento
            int freq = refdoc_get_freq(ref);
            if (fwrite(&freq, sizeof freq, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao salvar frequencia do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_salvaIndicePalavras");
            }

            float tfIdf = refdoc_get_tdIdf(ref);
            if (fwrite(&tfIdf, sizeof tfIdf, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao salvar tfIdf do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_salvaIndicePalavras");
            }
        }
        free(saveptr);
    }
    free(saveptr);
}

static HashTable *repoidx_carregaIndicePalavras(FILE *fbin) {

}

static HashTable *repoidx_carregaIndicePalavras(FILE *fbin) {

}

void repoidx_salvaIndice(Indice *idx, const char *arquivo) {
    FILE *fbin = fopen(arquivo, "wb");

    // HashTable<string, Documento>
    HashTable *idxDocumentos = indice_get_documentos(idx);

    repoidx_salvaIndiceDocumentos(idxDocumentos, fbin);

    // HashTable<string, Palavra>
    HashTable *idxPalavras = indice_get_palavras(idx);

    repoidx_salvaIndicePalavras(idxPalavras, fbin);

    fclose(fbin);
}

Indice *repoidx_carregaIndice(const char *arquivo) {
    FILE *fbin = fopen(arquivo, "wb");

    // HashTable<string, Documento>
    HashTable *idxDocumentos = repoidx_carregaIndiceDocumentos(fbin);    

    // HashTable<string, Palavra>
    HashTable *idxPalavras = repoidx_carregaIndicePalavras(fbin);

    fclose(fbin);

    Indice *idx = indice_init(idxDocumentos, idxPalavras);

    ht_dispose(idxDocumentos);
    ht_dispose(idxPalavras);

    return idx;
}
