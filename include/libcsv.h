/**
 * @file libcsv.h
 * @author Suraj Kareppagol (surajkareppagol.dev@gmail.com)
 * @brief Header file for libcsv
 *
 * libcsv is yet another CSV library written in C. The main goal of this library
 * is to get CSV data from a file and convert it into a C structure, which then
 * can be used in other applications.
 *
 * @version 0.1
 * @date 2025-01-03
 *
 * @copyright Copyright (c) 2025
 */

#ifndef LIBCSV
#define LIBCSV

#define CSV_BUFFER_SIZE 1024
#define CSV_DELIMETER ","

#define CSV_MAX_FIELDS 1024
#define CSV_FILE_NAME 1024

#define CSV_DEFAULT_FILE_NAME "output.csv"

#include <stdbool.h>

/************ TYPE BLOCKS ************/

typedef struct csv_char_block {
  struct csv_char_block *next_block;

  char *data;
} CSV_CHAR_BLOCK;

typedef struct csv_int_block {
  struct csv_int_block *next_block;

  int data;
} CSV_INT_BLOCK;

typedef struct csv_double_block {
  struct csv_double_block *next_block;

  double data;
} CSV_DOUBLE_BLOCK;

/************ FIELD BLOCK ************/

typedef enum { CHAR_TYPE, INT_TYPE, DOUBLE_TYPE } CSV_FIELD_TYPE;

typedef struct csv_field_list {
  char *field;

  CSV_FIELD_TYPE field_type;

  struct csv_char_block *char_block_head;
  struct csv_char_block *char_block_tail;

  struct csv_int_block *int_block_head;
  struct csv_int_block *int_block_tail;

  struct csv_double_block *double_block_head;
  struct csv_double_block *double_block_tail;
} CSV_FIELD_LIST;

/************ TOP BLOCK ************/

typedef struct csv_list {
  struct csv_field_list *field_list[CSV_MAX_FIELDS];

} CSV_LIST;

/************ METADATA BLOCK ************/

typedef struct csv_metadata {
  unsigned fields;
  unsigned items;
} CSV_METADATA;

/************ API ************/

/**
 * @brief Import data from CSV file into a C Structure
 *
 * @param csv_file
 * @param metadata
 * @return CSV_LIST*
 */
CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata);

/**
 * @brief Export C data structure into csv file
 *
 * @param data
 * @param csv_file
 * @param metadata
 */
void csv_export(CSV_LIST *csv_list, char *csv_file, CSV_METADATA *metadata);

/**
 * @brief Extract data from a specific field
 *
 * @param field
 * @param csv_list
 * @param metadata
 * @return void*
 */
void *csv_field(char *field, CSV_LIST *csv_list, CSV_METADATA *metadata);

/**
 * @brief Extract data from specific column
 *
 * @param column
 * @param csv_list
 * @param metadata
 * @return void*
 */
void *csv_column(unsigned int column, CSV_LIST *csv_list,
                 CSV_METADATA *metadata);

/**
 * @brief Add a row of data
 *
 * @param data
 * @param metadata
 */
void csv_add_row(char *data, CSV_LIST *csv_list, CSV_METADATA *metadata);

/**
 * @brief Remove a row of data
 *
 * @param data
 * @param csv_list
 * @param metadata
 */
void csv_remove_row(unsigned int row, CSV_LIST *csv_list,
                    CSV_METADATA *metadata);

void csv_row();

/**
 * @brief Show CSV data on to the terminal
 *
 * @param csv_list
 * @param metadata
 */
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata);

/**
 * @brief Free the memory allocated for csv_list
 *
 * @param csv_list
 * @param metadata
 */
void csv_clear(CSV_LIST *csv_list, CSV_METADATA *metadata);

#endif