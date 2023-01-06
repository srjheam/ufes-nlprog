#ifndef _HT_
#define _HT_

#include "lista.h"
#include "types.h"

typedef struct tHashTable HashTable;

/**
 * @brief Inicializa dinamicamente uma @ref HashTable
 *
 * @param comparadorChaves Uma função comparadora das chaves da @ref HashTable
 * @param liberaChaves Uma função destrutora das chaves da @ref HashTable
 * @param liberaValores Uma função destrutora dos valores da @ref HashTable
 * @return HashTable* Uma nova instancia de @ref HashTable
 */
HashTable *ht_init(cmp_fn comparadorChaves, free_fn liberaChaves,
                   free_fn liberaValores);

/**
 * @brief Libera o @p ht e todos os seus elementos da memória em que foram
 * dinamicamente alocados
 *
 * @param ht Uma instancia de @ref HashTable
 */
void ht_dispose(HashTable *ht);

/**
 * @brief Acessa o valor da @p ht pelo sua chave
 * 
 * @param ht Uma instancia de @ref HashTable
 * @param chave A chave nessa @p ht
 * @param copiaChave Uma função copiadora da chave da @p ht
 * @return void** Ponteiro para o valor nessa chave - valor pode ser nulo.
 */
void **ht_index(HashTable *ht, const void *chave, cpy_fn copiaChave);

/**
 * @brief Adquire todos os pares chave valor dessa @p ht
 * 
 * @param ht Uma instancia de @ref HashTable
 * @return Lista* Uma @ref Lista* de @ref KeyValuePair*
 */
Lista *ht_get_allkvps(HashTable *ht);

#endif
