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

/************ CSV UTILS ************/

/* TODO: Refactor code */
void csv_util_add_node(CSV_LIST *csv_list, unsigned field, void *data,
                       CSV_FIELD_TYPE csv_field_type) {

  switch (csv_field_type) {
  case CHAR_TYPE: {
    CSV_CHAR_BLOCK *block = calloc(1, sizeof(CSV_CHAR_BLOCK));

    block->next_block = NULL;
    block->data = (char *)data;

    /* -- Add node */

    CSV_CHAR_BLOCK **head = &csv_list->field_list[field]->char_block_head;
    CSV_CHAR_BLOCK **tail = &csv_list->field_list[field]->char_block_tail;

    if (*head == NULL) {
      *head = block;
      *tail = block;
    } else {
      (*tail)->next_block = block;
      *tail = block;
    }

    break;
  }
  case INT_TYPE: {
    CSV_INT_BLOCK *block = (CSV_INT_BLOCK *)calloc(1, sizeof(CSV_INT_BLOCK));

    block->next_block = NULL;
    block->data = *((int *)data);

    /* -- Add node */

    CSV_INT_BLOCK **head = &csv_list->field_list[field]->int_block_head;
    CSV_INT_BLOCK **tail = &csv_list->field_list[field]->int_block_tail;

    if (*head == NULL) {
      *head = block;
      *tail = block;
    } else {
      (*tail)->next_block = block;
      *tail = block;
    }

    break;
  }
  case DOUBLE_TYPE: {
    CSV_DOUBLE_BLOCK *block =
        (CSV_DOUBLE_BLOCK *)calloc(1, sizeof(CSV_DOUBLE_BLOCK));

    block->next_block = NULL;
    block->data = *((double *)data);

    /* -- Add node */

    CSV_DOUBLE_BLOCK **head = &csv_list->field_list[field]->double_block_head;
    CSV_DOUBLE_BLOCK **tail = &csv_list->field_list[field]->double_block_tail;

    if (*head == NULL) {
      *head = block;
      *tail = block;
    } else {
      (*tail)->next_block = block;
      *tail = block;
    }

    break;
  }
  }
}

/************ LIBCSV ************/

CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata) {
  FILE *csv_stream = fopen(csv_file, "r");

  bool fields_extracted = false;

  int total_fields = 0;
  int total_items = 0;

  /* -- Prepare for metadata extraction */
  if (metadata == NULL) {
    return NULL;
  }

  CSV_LIST *csv_list = calloc(1, sizeof(CSV_LIST));

  char csv_buffer[CSV_BUFFER_SIZE];
  memset(csv_buffer, 0, CSV_BUFFER_SIZE);

  while (fgets(csv_buffer, CSV_BUFFER_SIZE, csv_stream) != NULL) {
    /* -- First extract all fields */
    char *token = strtok(csv_buffer, CSV_DELIMETER);

    unsigned field = 0;

    while (token != NULL) {
      token = util_trim_string(token);

      /* -- Extract fields */
      if (fields_extracted == false) {
        csv_list->field_list[field] = calloc(1, sizeof(CSV_FIELD_LIST));
        csv_list->field_list[field]->field = token;

        token = strtok(NULL, CSV_DELIMETER);
        field += 1;
        total_fields += 1;

        continue;
      }

      int int_type = 0;
      double double_type = 0;

      /* -- Extract double data */
      if (util_string_to_double(token, &double_type) == 0) {
        csv_list->field_list[field]->field_type = DOUBLE_TYPE;
        csv_util_add_node(csv_list, field, &double_type, DOUBLE_TYPE);
      }
      /* -- Extract integer data */
      else if (util_string_to_number(token, &int_type) == 0) {
        csv_list->field_list[field]->field_type = INT_TYPE;
        csv_util_add_node(csv_list, field, &int_type, INT_TYPE);
      }
      /* -- Extract string data */
      else {
        csv_list->field_list[field]->field_type = CHAR_TYPE;
        csv_util_add_node(csv_list, field, token, CHAR_TYPE);
      }

      field += 1;

      token = strtok(NULL, CSV_DELIMETER);
    }

    total_items += 1;

    fields_extracted = true;
  }

  *metadata = calloc(1, sizeof(CSV_METADATA));

  (*metadata)->fields = total_fields;
  (*metadata)->items = total_items - 1;

  fclose(csv_stream);

  return csv_list;
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

/* TODO: Refactor code */
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata) {
  printf("+----------+\n"
         "| CSV INFO |\n"
         "+----------+\n");

  printf("\n"
         "+ Fields            : %d\n"
         "+ Items Per Columns : %d\n\n",
         metadata->fields, metadata->items);

  /* -- Print the field names */
  for (int i = 0; i < metadata->fields; i++) {
    printf("| %10s | ", csv_list->field_list[i]->field);
  }

  printf("\n\n");

  unsigned int row = 0;
  unsigned int field = 0;

  for (int i = 0; i < metadata->items + 1; i++) {
    for (int j = 0; j < metadata->fields; j++) {
      unsigned current_row = 0;

      CSV_FIELD_TYPE field_type = csv_list->field_list[j]->field_type;

      switch (field_type) {
      case CHAR_TYPE: {
        CSV_CHAR_BLOCK *block = csv_list->field_list[j]->char_block_head;

        for (int k = 0; k < metadata->items; k++) {
          if (current_row == row) {
            printf("| %10s | ", block->data);
            break;
          }
          current_row += 1;

          block = block->next_block;
        }
        break;
      }
      case INT_TYPE: {
        CSV_INT_BLOCK *block = csv_list->field_list[j]->int_block_head;

        for (int k = 0; k < metadata->items; k++) {
          if (current_row == row) {
            printf("| %10d | ", block->data);
            break;
          }
          current_row += 1;

          block = block->next_block;
        }
        break;
      }
      case DOUBLE_TYPE: {
        CSV_DOUBLE_BLOCK *block = csv_list->field_list[j]->double_block_head;

        for (int k = 0; k < metadata->items; k++) {
          if (current_row == row) {
            printf("| %10lf | ", block->data);
            break;
          }
          current_row += 1;

          block = block->next_block;
        }
        break;
      }
      }
    }

    row += 1;
    printf("\n");
  }
}