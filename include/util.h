#ifndef UTIL
#define UTIL

/************ UTILITY API ************/

/**
 * @brief Remove left and right spaces
 *
 * @param string
 * @return char*
 */
char *util_trim_string(char *string);

/**
 * @brief Convert string to number
 *
 * @param string
 * @param data
 * @return int
 */
int util_string_to_number(char *string, int *data);

/**
 * @brief Convert string to double
 *
 * @param string
 * @param data
 * @return int
 */
int util_string_to_double(char *string, double *data);

/**
 * @brief Extract and return total number of fields
 *
 * @param string
 * @return int
 */
int util_total_fields(char *string);

#endif