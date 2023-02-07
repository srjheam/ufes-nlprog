#ifndef _KVP_
#define _KVP_

#include <stdbool.h>

#include "types.h"

typedef struct tKeyValuePair KeyValuePair;

/**
 * @brief Inicializa dinamicamente um @ref KeyValuePair
 *
 * @param chave A chave
 * @param valor O valor
 * @param cpyKey Uma função copiadora da chave do @ref KeyValuePair
 * @param cpyValue Uma função copiadora do valor do @ref KeyValuePair
 * @param liberaChave Uma função destrutora da chave do @ref KeyValuePair
 * @param liberaValor Uma função destrutora do valor do @ref KeyValuePair
 * @return KeyValuePair* Uma nova instancia de @ref KeyValuePair
 */
KeyValuePair *kvp_init(const void *chave, const void *valor, cpy_fn cpyKey,
                       cpy_fn cpyValue, free_fn liberaChave,
                       free_fn liberaValor);

/**
 * @brief Libera o @p pair e todos os seus elementos da memória em que foram
 * dinamicamente alocados
 *
 * @param pair Uma instancia de @ref KeyValuePair
 */
void kvp_dispose(KeyValuePair *pair);

/**
 * @brief Adquire a chave de @p pair
 *
 * @param pair Uma instancia de @ref KeyValuePair
 * @return const void* A chave de @p pair
 */
const void *kvp_get_key(const KeyValuePair *pair);

/**
 * @brief Adquire o valor de @p pair
 *
 * @param pair Uma instancia de @ref KeyValuePair
 * @return const void* O valor de @p pair
 */
void *kvp_get_value(const KeyValuePair *pair);

void kvp_set_value(KeyValuePair *pair, const void *value);

bool kvp_get_is_removed(const KeyValuePair *pair);

/**
 * @brief Marca @p pair como removido
 *
 * @param pair Uma instancia de @ref KeyValuePair
 */
void kvp_set_removed(KeyValuePair *pair, bool removed);

/**
 * @brief Efetua uma copia do @ref KeyValuePair* @p kvp
 *
 * @param kvp O @ref KeyValuePair*
 * @return KeyValuePair* Uma nova instancia de @ref KeyValuePair* identica à
 * original
 */
KeyValuePair *kvp_cpy(KeyValuePair *kvp);

#endif
