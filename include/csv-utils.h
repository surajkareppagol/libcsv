/**
 * @file csv-utils.h
 * @author Suraj Kareppagol (surajkareppagol.dev@gmail.com)
 * @brief Header file for csv utils
 * @version 0.1
 * @date 2025-01-09
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef CSV_UTILS
#define CSV_UTILS

#include <libcsv.h>

void csv_util_add_node(CSV_LIST *csv_list, unsigned field, void *data,
                       CSV_FIELD_TYPE csv_field_type);

#endif