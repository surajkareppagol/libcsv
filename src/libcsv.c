#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcsv.h>
#include <util.h>

CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata) {
  FILE *csv_stream = fopen(csv_file, "r");

  bool extract_metadata = false;
  bool fields_extracted = false;

  if (metadata != NULL || *metadata != NULL) {
    extract_metadata = true;
  }

  if (extract_metadata) {
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

      if (extract_metadata) {
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

      /* -- Extract data */

      CSV_STRING_BLOCK *block = calloc(1, sizeof(CSV_STRING_BLOCK));
      block->next_block = NULL;
      block->data = token;

      if (csv_l->field_list[field]->string_block_head == NULL) {
        csv_l->field_list[field]->string_block_head = block;
        csv_l->field_list[field]->string_block_tail = block;
      } else {
        csv_l->field_list[field]->string_block_tail->next_block = block;
        csv_l->field_list[field]->string_block_tail = block;
      }

      field += 1;

      token = strtok(NULL, CSV_DELIMETER);
    }

    fields_extracted = true;
    extract_metadata = false;
  }

  fclose(csv_stream);

  return csv_l;
}