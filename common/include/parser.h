#ifndef _PARSER_
#define _PARSER_

#include <stdbool.h>

/**
 * @brief Tenta converter uma dada @p str para @ref int; e, se valida, passa o
 * valor por referencia para @p val
 *
 * @param str Um @ref int como uma string
 * @param val Ponteiro onde sera guadado o resultado da conversao; zero se
 * falhou
 * @return bool true, se a conversao foi feita com sucesso; do contrario, false
 */
bool tryParseInt(const char *str, int *val);

/**
 * @brief Tenta converter uma dada @p str para @ref float; e, se valida, passa o
 * valor por referencia para @p val
 *
 * @param str Um @ref float como uma string
 * @param val Ponteiro onde sera guadado o resultado da conversao; zero se
 * falhou
 * @return bool true, se a conversao foi feita com sucesso; do contrario, false
 */
bool tryParseFloat(const char *str, float *val);

int *parseInt(const char *str);

#endif
