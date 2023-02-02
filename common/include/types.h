#ifndef _TYPES_
#define _TYPES_

#include <stdlib.h>

/**
 * @brief Tipo comum de uma função que libera uma instância qualquer alocado
 * dinâmicamente na memória.
 *
 */
typedef void (*free_fn)(void *);

/**
 * @brief Tipo comum de uma função que compara duas instâncias.
 *
 */
typedef int (*cmp_fn)(const void *, const void *);

/**
 * @brief Tipo comum de uma função que copia uma instância.
 *
 */
typedef void *(*cpy_fn)(const void *);

/**
 * @brief Tipo comum de uma função que calcula o hash de uma instância.
 * 
 */
typedef size_t (*hash_fn)(const void *);

#endif
