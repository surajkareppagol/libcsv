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
#include <stdio.h>

/**
 * @brief CSV utility function to add a node to CSV_LIST
 *
 * @param csv_list
 * @param field
 * @param data
 * @param csv_field_type
 */
void csv_util_add_node(CSV_LIST *csv_list, unsigned field, void *data,
                       CSV_FIELD_TYPE csv_field_type);

/**
 * @brief CSV utility function to print data based on the stream
 *
 * @param csv_list
 * @param csv_stream
 * @param metadata
 */
void csv_util_show(CSV_LIST *csv_list, FILE *csv_stream,
                   CSV_METADATA *metadata);

#endif