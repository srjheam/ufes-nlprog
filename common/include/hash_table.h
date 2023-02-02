/**
 * @file hash_table.c
 * @brief Generic, openly addressable, dynamically resizable hash table
 * implementation
 * @see https://en.wikipedia.org/wiki/Hash_table#Collision_resolution
 * @version 0.1
 * @date 2023-02-01
 *
 * @copyright Copyright (c) 2023
 *
 *
 */

#ifndef _HT_
#define _HT_

#include "key_value_pair.h"
#include "lista.h"
#include "types.h"

typedef struct tHashTable HashTable;

/**
 * @brief Inicializa dinamicamente uma @ref HashTable
 *
 * @param hashKey Uma função hash da chave da @p ht
 * @param cpyKey Uma função copiadora da chave da @p ht
 * @param cpyValue Uma função copiadora do valor da @p ht
 * @param cmpKey Uma função comparadora das chaves da @ref HashTable
 * @param disposeKey Uma função destrutora das chaves da @ref HashTable
 * @param disposeValue Uma função destrutora dos valores da @ref HashTable
 * @return HashTable* Uma nova instancia de @ref HashTable
 */
HashTable *ht_init(hash_fn hashKey, cpy_fn cpyKey, cpy_fn cpyValue,
                   cmp_fn cmpKey, free_fn disposeKey,
                   free_fn disposeValue);

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

size_t ht_get_qty(HashTable *ht);

/**
 * @brief Adquire todos os pares chave valor dessa @p ht
 *
 * @param ht Uma instancia de @ref HashTable
 * @return Lista* Uma @ref Lista* de @ref KeyValuePair*
 */
Lista *ht_to_list(HashTable *ht);

KeyValuePair *ht_iter(const HashTable *ht, void **saveptr);

HashTable *ht_cpy(const HashTable *ht);

#endif
