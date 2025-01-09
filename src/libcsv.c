/**
 * @file libcsv.h
 * @author Suraj Kareppagol (surajkareppagol.dev@gmail.com)
 * @brief Source file for libcsv
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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcsv.h>
#include <util.h>

CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata) {
  FILE *csv_stream = fopen(csv_file, "r");

  bool metadata_extracted = false;
  bool fields_extracted = false;

  if (metadata != NULL || *metadata != NULL) {
    metadata_extracted = true;
  }

  if (metadata_extracted) {
    *metadata = calloc(1, sizeof(CSV_METADATA));
    (*metadata)->fields = 0;
  }

  CSV_LIST *csv_l = calloc(1, sizeof(CSV_LIST));

  char csv_buffer[CSV_BUFFER_SIZE];

  memset(csv_buffer, 0, CSV_BUFFER_SIZE);

  while (fgets(csv_buffer, CSV_BUFFER_SIZE, csv_stream) != NULL) {
    /* -- First line - All fields are available */
    char *token = strtok(csv_buffer, CSV_DELIMETER);

    unsigned int field = 0;

    while (token != NULL) {
      token = util_trim_string(token);

      if (metadata_extracted) {
        (*metadata)->fields += 1;
      }

      /* -- Extract fields */
      if (fields_extracted == false) {
        csv_l->field_list[field] = calloc(1, sizeof(CSV_FIELD_LIST));
        csv_l->field_list[field]->field = token;

        token = strtok(NULL, CSV_DELIMETER);
        field += 1;

        continue;
      }

      int int_type = 0;
      double double_type = 0;

      /* -- Extract double data */
      if (util_string_to_double(token, &double_type) == 0) {
        csv_l->field_list[field]->field_type.double_type = true;

        CSV_DOUBLE_BLOCK *block = calloc(1, sizeof(CSV_DOUBLE_BLOCK));

        block->next_block = NULL;
        block->data = double_type;

        if (csv_l->field_list[field]->double_block_head == NULL) {
          csv_l->field_list[field]->double_block_head = block;
          csv_l->field_list[field]->double_block_tail = block;
        } else {
          csv_l->field_list[field]->double_block_tail->next_block = block;
          csv_l->field_list[field]->double_block_tail = block;
        }
      }

      /* -- Extract integer data */
      else if (util_string_to_number(token, &int_type) == 0) {
        csv_l->field_list[field]->field_type.int_type = true;

        CSV_INT_BLOCK *block = calloc(1, sizeof(CSV_INT_BLOCK));

        block->next_block = NULL;
        block->data = int_type;

        if (csv_l->field_list[field]->int_block_head == NULL) {
          csv_l->field_list[field]->int_block_head = block;
          csv_l->field_list[field]->int_block_tail = block;
        } else {
          csv_l->field_list[field]->int_block_tail->next_block = block;
          csv_l->field_list[field]->int_block_tail = block;
        }
      }

      /* -- Extract string data */
      else {
        csv_l->field_list[field]->field_type.char_type = true;

        CSV_CHAR_BLOCK *block = calloc(1, sizeof(CSV_CHAR_BLOCK));
        block->next_block = NULL;
        block->data = token;

        if (csv_l->field_list[field]->char_block_head == NULL) {
          csv_l->field_list[field]->char_block_head = block;
          csv_l->field_list[field]->char_block_tail = block;
        } else {
          csv_l->field_list[field]->char_block_tail->next_block = block;
          csv_l->field_list[field]->char_block_tail = block;
        }
      }

      field += 1;

      token = strtok(NULL, CSV_DELIMETER);
    }

    fields_extracted = true;
    metadata_extracted = false;
  }

  fclose(csv_stream);

  return csv_l;
}

void csv_export(CSV_LIST *csv_list, char *csv_file, CSV_METADATA *metadata) {
  char *csv_filename = calloc(1, CSV_FILE_NAME);

  /* -- Get CSV file name */
  if (csv_file == NULL) {
    strncpy(csv_filename, CSV_DEFAULT_FILE_NAME, strlen(CSV_DEFAULT_FILE_NAME));
  } else {
    strncpy(csv_filename, csv_file, strlen(csv_file));
  }

  FILE *csv_stream = fopen(csv_filename, "w");

  /* -- First print fields */
  for (int i = 0; i < metadata->fields; i++) {
    fprintf(csv_stream, "%s%s", csv_list->field_list[i]->field,
            (i != metadata->fields - 1) ? "," : "");
  }

  fprintf(csv_stream, "\n");

  unsigned int row = 0;
  unsigned int field = 0;
  bool last_row = false;

  /* -- Start from first row print all the values then go to next row and so
   *    on until the last row */
  while (1) {
    for (int i = 0; i < metadata->fields; i++) {
      unsigned current_row = 0;
      CSV_CHAR_BLOCK *block = csv_list->field_list[i]->char_block_head;

      while (block != NULL) {
        if (current_row == row) {
          fprintf(csv_stream, "%s%s", block->data,
                  (i != metadata->fields - 1) ? "," : "");
          break;
        }
        current_row += 1;

        block = block->next_block;

        if (block == NULL) {
          last_row = true;
        }
      }
    }

    row += 1;
    fprintf(csv_stream, "\n");

    if (last_row) {
      break;
    }
  }

  fclose(csv_stream);
}

CSV_FIELD_LIST *csv_field(char *field, CSV_LIST *csv_list,
                          CSV_METADATA *metadata) {
  for (int i = 0; i < metadata->fields; i++) {
    if (strcmp(field, csv_list->field_list[i]->field) == 0) {
      return csv_list->field_list[i];
    }
  }

  return NULL;
}

CSV_FIELD_LIST *csv_column(unsigned int column, CSV_LIST *csv_list,
                           CSV_METADATA *metadata) {
  for (int i = 0; i < metadata->fields; i++) {
    if (i == column) {
      return csv_list->field_list[i];
    }
  }

  return NULL;
}

void csv_add_row(char *data, CSV_LIST *csv_list, CSV_METADATA *metadata) {
  char *token = strtok(data, CSV_DELIMETER);

  unsigned int field = 0;

  while (token != NULL) {
    token = util_trim_string(token);

    /* -- Extract data */

    CSV_CHAR_BLOCK *block = calloc(1, sizeof(CSV_CHAR_BLOCK));
    block->next_block = NULL;
    block->data = token;

    csv_list->field_list[field]->char_block_tail->next_block = block;
    csv_list->field_list[field]->char_block_tail = block;

    field += 1;

    token = strtok(NULL, CSV_DELIMETER);
  }
}

void csv_remove_row(unsigned int row, CSV_LIST *csv_list,
                    CSV_METADATA *metadata) {
  for (int i = 0; i < metadata->fields; i++) {
    unsigned current_row = 0;
    CSV_CHAR_BLOCK *block = csv_list->field_list[i]->char_block_head;
    CSV_CHAR_BLOCK *previous_block = block;

    /* -- Remove first row */
    if (row == 0) {
      csv_list->field_list[i]->char_block_head = block->next_block;

      free(block);
      continue;
    }

    while (block != NULL) {
      if (current_row == row) {
        previous_block->next_block = block->next_block;

        block->next_block = NULL;
        free(block);

        break;
      }

      previous_block = block;
      block = block->next_block;

      current_row += 1;
    }
  }
}

void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata) {
  printf("\nCSV file has %d fields.\n\n", metadata->fields);

  for (int i = 0; i < metadata->fields; i++) {
    printf("| %10s | ", csv_list->field_list[i]->field);

    printf("CHAR: %d, ", csv_list->field_list[i]->field_type.char_type);
    printf("DOUBLE: %d, ", csv_list->field_list[i]->field_type.double_type);
    printf("INT: %d\n", csv_list->field_list[i]->field_type.int_type);
  }

  printf("\n\n");

  unsigned int row = 0;
  unsigned int field = 0;
  bool last_row = false;

  while (1) {
    for (int i = 0; i < metadata->fields; i++) {
      unsigned current_row = 0;
      CSV_CHAR_BLOCK *char_block = NULL;
      CSV_INT_BLOCK *int_block = NULL;
      CSV_DOUBLE_BLOCK *double_block = NULL;

      CSV_FIELD_TYPE field_type;

      if (csv_list->field_list[i]->field_type.char_type) {
        char_block = csv_list->field_list[i]->char_block_head;

        while (char_block != NULL) {
          if (current_row == row) {
            printf("| %10s | ", char_block->data);
            break;
          }
          current_row += 1;

          char_block = char_block->next_block;

          if (char_block == NULL) {
            last_row = true;
          }
        }
      } else if (csv_list->field_list[i]->field_type.int_type) {

        int_block = csv_list->field_list[i]->int_block_head;

        while (int_block != NULL) {
          if (current_row == row) {
            printf("| %10d | ", int_block->data);
            break;
          }
          current_row += 1;

          int_block = int_block->next_block;

          if (int_block == NULL) {
            last_row = true;
          }
        }
      } else {

        double_block = csv_list->field_list[i]->double_block_head;

        while (double_block != NULL) {
          if (current_row == row) {
            printf("| %10lf | ", double_block->data);
            break;
          }
          current_row += 1;

          double_block = double_block->next_block;

          if (double_block == NULL) {
            last_row = true;
          }
        }
      }
    }

    row += 1;
    printf("\n");

    if (last_row) {
      break;
    }
  }
}