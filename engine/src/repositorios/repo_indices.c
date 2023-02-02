#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "documento.h"
#include "exception.h"
#include "palavra.h"
#include "ref_documento.h"
#include "ref_palavra.h"

#include "repo_indices.h"

static void repoidx_salvaIndiceDocumentos(HashTable *idxDocumentos,
                                          FILE *fbin) {
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

static HashTable *repoidx_carregaIndiceDocumentos(FILE *fbin) {
    int docQty = 0;
    size_t qsize = sizeof docQty;
    if (fread(&docQty, qsize, 1, fbin) != 1) {
        exception_throw_failure("Erro ao carregar qtyDoc do indice no bin - em "
                                "engine/repositorios/"
                                "repo_indices.repoidx_carregaIndiceDocumentos");
    }

    // HashTable<string, Documento>
    HashTable *idxDocumentos =
        ht_init((cpy_fn)strdup, (cpy_fn)doc_cpy, (cmp_fn)strcmp, (free_fn)free,
                (free_fn)doc_dispose);

    for (int i = 0; i < docQty; i++) {
        char *nome = NULL;
        size_t nlen = 0;
        if (getdelim(&nome, &nlen, '\0', fbin) == -1) {
            exception_throw_failure(
                "Erro ao carregar nomeDoc do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_carregaIndiceDocumentos");
        }

        char *classe = NULL;
        nlen = 0;
        if (getdelim(&classe, &nlen, '\0', fbin) == -1) {
            exception_throw_failure(
                "Erro ao carregar classe do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_carregaIndiceDocumentos");
        }

        int refPalQty = 0;
        size_t qsize = sizeof refPalQty;
        if (fread(&refPalQty, qsize, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao carregar refPalQty do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_carregaIndiceDocumentos");
        }

        // HashTable<string, RefPalavra>
        HashTable *refPalavras =
            ht_init((cpy_fn)strdup, (cpy_fn)refpalavra_cpy, (cmp_fn)strcmp,
                    (free_fn)free, (free_fn)refpalavra_dispose);

        for (int j = 0; j < refPalQty; j++) {
            char *palavra = NULL;
            size_t nlen = 0;
            if (getdelim(&palavra, &nlen, '\0', fbin) == -1) {
                exception_throw_failure(
                    "Erro ao carregar palavra do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_carregaIndiceDocumentos");
            }

            int freq = 0;
            size_t qsize = sizeof freq;
            if (fread(&freq, qsize, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao carregar freq do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_carregaIndiceDocumentos");
            }

            RefPalavra *ref = refpalavra_init(palavra, freq);

            ht_add(refPalavras, palavra, ref);

            free(palavra);
            refpalavra_dispose(ref);
        }

        Documento *doc = doc_init(nome, classe, refPalavras);

        free(classe);
        ht_dispose(refPalavras);

        ht_add(idxDocumentos, nome, doc);

        free(nome);
        doc_dispose(doc);
    }

    return idxDocumentos;
}

static HashTable *repoidx_carregaIndicePalavras(FILE *fbin) {
    int palQty = 0;
    size_t qsize = sizeof palQty;
    if (fread(&palQty, qsize, 1, fbin) != 1) {
        exception_throw_failure("Erro ao carregar palQty do indice no bin - em "
                                "engine/repositorios/"
                                "repo_indices.repoidx_carregaIndicePalavras");
    }

    // HashTable<string, Palavra>
    HashTable *idxPalavras =
        ht_init((cpy_fn)strdup, (cpy_fn)palavra_cpy, (cmp_fn)strcmp,
                (free_fn)free, (free_fn)palavra_dispose);

    for (int i = 0; i < palQty; i++) {
        char *palavra = NULL;
        size_t nlen = 0;
        if (getdelim(&palavra, &nlen, '\0', fbin) == -1) {
            exception_throw_failure(
                "Erro ao carregar palavra do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_carregaIndicePalavras");
        }

        int refDocQty = 0;
        size_t qsize = sizeof refDocQty;
        if (fread(&refDocQty, qsize, 1, fbin) != 1) {
            exception_throw_failure(
                "Erro ao carregar refDocQty do indice no bin - em "
                "engine/repositorios/"
                "repo_indices.repoidx_carregaIndicePalavras");
        }

        // HashTable<string, RefDocumento>
        HashTable *refDocumentos =
            ht_init((cpy_fn)strdup, (cpy_fn)refdoc_cpy, (cmp_fn)strcmp,
                    (free_fn)free, (free_fn)refdoc_dispose);

        for (int j = 0; j < refDocQty; j++) {
            char *documento = NULL;
            size_t nlen = 0;
            if (getdelim(&documento, &nlen, '\0', fbin) == -1) {
                exception_throw_failure(
                    "Erro ao carregar documento do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_carregaIndicePalavras");
            }

            int freq = 0;
            qsize = sizeof freq;
            if (fread(&freq, qsize, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao carregar freq do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_carregaIndicePalavras");
            }

            float tfIdf = 0;
            size_t qsize = sizeof tfIdf;
            if (fread(&tfIdf, qsize, 1, fbin) != 1) {
                exception_throw_failure(
                    "Erro ao carregar tfIdf do indice no bin - em "
                    "engine/repositorios/"
                    "repo_indices.repoidx_carregaIndicePalavras");
            }

            RefDocumento *ref = refdoc_init(documento, freq, tfIdf);

            ht_add(refDocumentos, documento, ref);

            free(documento);
            refdoc_dispose(ref);
        }

        Palavra *pal = palavra_init(palavra, refDocumentos);

        ht_dispose(refDocumentos);

        ht_add(idxPalavras, palavra, pal);

        free(palavra);
        palavra_dispose(pal);
    }

    return idxPalavras;
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
    FILE *fbin = fopen(arquivo, "rb");
    if (fbin == NULL) {
        exception_throw_failure("Erro ao abrir arquivo de indice - em "
                                "engine/repositorios/"
                                "repo_indices.repoidx_carregaIndice");
    }

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
