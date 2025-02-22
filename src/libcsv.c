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

/************************************************/
/*             CSV_UTIL_ADD_NODE                */
/************************************************/

/* TODO: Refactor code */
void csv_util_add_node(CSV_LIST *csv_list, unsigned field, void *data,
                       CSV_FIELD_TYPE csv_field_type) {

  switch (csv_field_type) {
  case CHAR_TYPE: {
    CSV_CHAR_BLOCK *block = calloc(1, sizeof(CSV_CHAR_BLOCK));

    if (block == NULL) {
      fprintf(stderr, "%s: Memory allocation failed.\n", __func__);
      return;
    }

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

    if (block == NULL) {
      fprintf(stderr, "%s: Memory allocation failed.\n", __func__);
      return;
    }

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

    if (block == NULL) {
      fprintf(stderr, "%s: Memory allocation failed.\n", __func__);
      return;
    }

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

/************************************************/
/*             CSV_UTIL_SHOW                    */
/************************************************/

void csv_util_show(CSV_LIST *csv_list, FILE *csv_stream,
                   CSV_METADATA *metadata) {

  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return;
  }

  /* -- First print fields */
  for (int i = 0; i < metadata->fields; i++) {
    fprintf(csv_stream, "%s%s", csv_list->field_list[i]->field,
            (i != metadata->fields - 1) ? "," : "");
  }

  fprintf(csv_stream, "\n");

  /* -- Print remaining data */
  unsigned int row = 0;

  for (int i = 0; i < metadata->items; i++) {
    for (int j = 0; j < metadata->fields; j++) {
      unsigned current_row = 0;

      CSV_FIELD_TYPE field_type = csv_list->field_list[j]->field_type;

      switch (field_type) {
      case CHAR_TYPE: {
        CSV_CHAR_BLOCK *block = csv_list->field_list[j]->char_block_head;

        for (int k = 0; k < metadata->items; k++) {
          if (current_row == row) {
            fprintf(csv_stream, "%s%s", block->data,
                    (j != metadata->fields - 1) ? "," : "");
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
            fprintf(csv_stream, "%d%s", block->data,
                    (j != metadata->fields - 1) ? "," : "");
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
            fprintf(csv_stream, "%.2lf%s", block->data,
                    (j != metadata->fields - 1) ? "," : "");
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
    fprintf(csv_stream, "\n");
  }
}

/************************************************/
/*             CSV_IMPORT                       */
/************************************************/

CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata) {
  FILE *csv_stream = fopen(csv_file, "r");

  if (csv_stream == NULL) {
    return NULL;
  }

  bool fields_extracted = false;

  int total_fields = 0;
  int total_items = 0;

  /* -- Prepare for metadata extraction */
  if (metadata == NULL) {
    return NULL;
  }

  CSV_LIST *csv_list = calloc(1, sizeof(CSV_LIST));
  *metadata = calloc(1, sizeof(CSV_METADATA));

  char csv_buffer[CSV_BUFFER_SIZE];
  memset(csv_buffer, 0, CSV_BUFFER_SIZE);

  while (fgets(csv_buffer, CSV_BUFFER_SIZE, csv_stream) != NULL) {
    /* -- Check for correct number of fields */
    if (fields_extracted == true &&
        (util_total_fields(csv_buffer) != (*metadata)->fields)) {
      continue;
    }

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

    (*metadata)->fields = total_fields;
  }

  (*metadata)->items = total_items - 1;

  fclose(csv_stream);

  return csv_list;
}

/************************************************/
/*             CSV_EXPORT                       */
/************************************************/

void csv_export(CSV_LIST *csv_list, CSV_METADATA *metadata, char *output) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return;
  }

  FILE *csv_stream;

  /* -- Get CSV file name */
  csv_stream = fopen(output, "w");

  /* -- Save data to a file */
  csv_util_show(csv_list, csv_stream, metadata);

  fclose(csv_stream);
}

/************************************************/
/*             CSV_FIELD                       */
/************************************************/

void *csv_field(char *field, CSV_LIST *csv_list, CSV_METADATA *metadata) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return NULL;
  }

  for (int i = 0; i < metadata->fields; i++) {
    if (strcmp(field, csv_list->field_list[i]->field) == 0) {
      CSV_FIELD_TYPE field_type = csv_list->field_list[i]->field_type;

      switch (field_type) {
      case CHAR_TYPE: {
        return csv_list->field_list[i]->char_block_head;
      }
      case INT_TYPE: {
        return csv_list->field_list[i]->int_block_head;
      }
      case DOUBLE_TYPE: {
        return csv_list->field_list[i]->double_block_head;
      }
      }
    }
  }

  return NULL;
}

/************************************************/
/*             CSV_COLUMN                       */
/************************************************/

void *csv_column(unsigned int column, CSV_LIST *csv_list,
                 CSV_METADATA *metadata) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return NULL;
  }

  for (int i = 0; i < metadata->fields; i++) {
    if (i == column) {
      CSV_FIELD_TYPE field_type = csv_list->field_list[i]->field_type;

      switch (field_type) {
      case CHAR_TYPE: {
        return csv_list->field_list[i]->char_block_head;
      }
      case INT_TYPE: {
        return csv_list->field_list[i]->int_block_head;
      }
      case DOUBLE_TYPE: {
        return csv_list->field_list[i]->double_block_head;
      }
      }
    }
  }

  return NULL;
}

/************************************************/
/*             CSV_ADD_ROW                      */
/************************************************/

void csv_add_row(char *data, CSV_LIST *csv_list, CSV_METADATA *metadata) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return;
  }

  if (util_total_fields(data) != metadata->fields) {
    return;
  }

  char *token = strtok(data, CSV_DELIMETER);

  unsigned field = 0;

  while (token != NULL) {
    token = util_trim_string(token);

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

  metadata->items += 1;
}

/************************************************/
/*             CSV_REMOVE_ROW                   */
/************************************************/

void csv_remove_row(unsigned int row, CSV_LIST *csv_list,
                    CSV_METADATA *metadata) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return;
  }

  for (int i = 0; i < metadata->fields; i++) {
    unsigned current_row = 0;

    CSV_FIELD_TYPE field_type = csv_list->field_list[i]->field_type;

    switch (field_type) {
    case CHAR_TYPE: {
      CSV_CHAR_BLOCK *block = csv_list->field_list[i]->char_block_head;
      CSV_CHAR_BLOCK *previous_block = block;

      /* -- Remove first row */
      if (row == 0) {
        csv_list->field_list[i]->char_block_head = block->next_block;

        free(block->data);
        free(block);
        continue;
      }

      while (block != NULL) {
        if (current_row == row) {
          previous_block->next_block = block->next_block;

          block->next_block = NULL;
          free(block->data);
          free(block);

          break;
        }

        previous_block = block;
        block = block->next_block;

        current_row += 1;
      }

      break;
    }

    case INT_TYPE: {
      CSV_INT_BLOCK *block = csv_list->field_list[i]->int_block_head;
      CSV_INT_BLOCK *previous_block = block;

      /* -- Remove first row */
      if (row == 0) {
        csv_list->field_list[i]->int_block_head = block->next_block;

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

      break;
    }

    case DOUBLE_TYPE: {
      CSV_DOUBLE_BLOCK *block = csv_list->field_list[i]->double_block_head;
      CSV_DOUBLE_BLOCK *previous_block = block;

      /* -- Remove first row */
      if (row == 0) {
        csv_list->field_list[i]->double_block_head = block->next_block;

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

      break;
    }
    }
  }

  metadata->items -= 1;
}

/************************************************/
/*             CSV_SHOW                         */
/************************************************/

/* TODO: Refactor code */
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata) {
  /* -- Print data to terminal */
  csv_util_show(csv_list, stdout, metadata);
}

/************************************************/
/*             CSV_CLEAR                        */
/************************************************/

void csv_clear(CSV_LIST *csv_list, CSV_METADATA *metadata) {
  if (csv_list == NULL || metadata == NULL) {
    fprintf(stderr, "%s: csv_list or metadata is NULL.\n", __func__);
    return;
  }

  for (int i = 0; i < metadata->fields; i++) {
    CSV_FIELD_TYPE field_type = csv_list->field_list[i]->field_type;

    switch (field_type) {
    case CHAR_TYPE: {
      CSV_CHAR_BLOCK *next_block = csv_list->field_list[i]->char_block_head;
      CSV_CHAR_BLOCK *current_block = next_block;

      for (int j = 0; j < metadata->items; j++) {
        next_block = current_block->next_block;

        free(current_block->data);
        free(current_block);

        current_block = next_block;
      }

      csv_list->field_list[i]->char_block_head = NULL;

      break;
    }
    case INT_TYPE: {
      CSV_INT_BLOCK *next_block = csv_list->field_list[i]->int_block_head;
      CSV_INT_BLOCK *current_block = next_block;

      for (int j = 0; j < metadata->items; j++) {
        next_block = current_block->next_block;

        free(current_block);

        current_block = next_block;
      }

      csv_list->field_list[i]->int_block_head = NULL;

      break;
    }
    case DOUBLE_TYPE: {
      CSV_DOUBLE_BLOCK *next_block = csv_list->field_list[i]->double_block_head;
      CSV_DOUBLE_BLOCK *current_block = next_block;

      for (int j = 0; j < metadata->items; j++) {
        next_block = current_block->next_block;

        free(current_block);

        current_block = next_block;
      }

      csv_list->field_list[i]->double_block_head = NULL;

      break;
    }
    }

    free(csv_list->field_list[i]->field);
    free(csv_list->field_list[i]);
  }

  free(csv_list);
  free(metadata);
}
