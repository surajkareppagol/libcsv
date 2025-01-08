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

/************ Data Structures ************/

typedef struct csv_string_block {
  struct csv_string_block *next_block;

  char *data;
} CSV_STRING_BLOCK;

typedef struct csv_numeric_block {
  struct csv_numeric_block *next_block;

  int data;
} CSV_NUMERIC_BLOCK;

typedef struct csv_field_list {
  char *field;

  struct csv_string_block *string_block_head;
  struct csv_numeric_block *numeric_block_head;

  struct csv_string_block *string_block_tail;
  struct csv_numeric_block *numeric_block_tail;
} CSV_FIELD_LIST;

/* Top node -> csv_field_list */
typedef struct csv_list {
  struct csv_field_list *field_list[CSV_MAX_FIELDS];

} CSV_LIST;

/* -- Metadata */
typedef struct csv_metadata {
  unsigned int fields;
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
 * @return CSV_FIELD_LIST*
 */
CSV_FIELD_LIST *csv_field(char *field, CSV_LIST *csv_list,
                          CSV_METADATA *metadata);

/**
 * @brief Extract data from specific column.
 *
 * @param column
 * @param csv_list
 * @param metadata
 * @return CSV_FIELD_LIST*
 */
CSV_FIELD_LIST *csv_column(unsigned int column, CSV_LIST *csv_list,
                           CSV_METADATA *metadata);

/**
 * @brief Add a row of data.
 *
 * @param data
 * @param metadata
 */
void csv_add_row(char *data, CSV_LIST *csv_list, CSV_METADATA *metadata);

/**
 * @brief Remove a row of data.
 *
 * @param data
 * @param csv_list
 * @param metadata
 */
void csv_remove_row(unsigned int row, CSV_LIST *csv_list,
                    CSV_METADATA *metadata);

void csv_row();

/**
 * @brief Show CSV data on to the terminal.
 *
 * @param csv_list
 * @param metadata
 */
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata);

#endif