#ifndef _HT_
#define _HT_

#include "key_value_pair.h"
#include "lista.h"
#include "types.h"

typedef struct tHashTable HashTable;

/**
 * @brief Inicializa dinamicamente uma @ref HashTable
 *
 * @param copiaChave Uma função copiadora da chave da @p ht
 * @param copiaValor Uma função copiadora do valor da @p ht
 * @param comparadorChaves Uma função comparadora das chaves da @ref HashTable
 * @param liberaChaves Uma função destrutora das chaves da @ref HashTable
 * @param liberaValores Uma função destrutora dos valores da @ref HashTable
 * @return HashTable* Uma nova instancia de @ref HashTable
 */
HashTable *ht_init(cpy_fn copiaChave, cpy_fn copiaValor,
                   cmp_fn comparadorChaves, free_fn liberaChaves,
                   free_fn liberaValores);

/**
 * @brief Libera o @p ht e todos os seus elementos da memória em que foram
 * dinamicamente alocados
 *
 * @param ht Uma instancia de @ref HashTable
 */
void ht_dispose(HashTable *ht);

/**
 * @brief Acessa o valor da @p ht pelo sua chave e armazena @p value nele
 *
 * @param ht Uma instancia de @ref HashTable
 * @param chave A chave nessa @p ht
 * @param value Valor nessa chave
 */
void ht_add(HashTable *ht, const void *chave, const void *value);

void *ht_get(HashTable *ht, const void *chave);

int ht_get_qty(HashTable *ht);

/**
 * @brief Adquire todos os pares chave valor dessa @p ht
 *
 * @param ht Uma instancia de @ref HashTable
 * @return Lista* Uma @ref Lista* de @ref KeyValuePair*
 */
Lista *ht_get_allkvps(HashTable *ht);

KeyValuePair *ht_iter(HashTable *ht, int *saveptr);

HashTable *ht_cpy(const HashTable *ht);

#endif
