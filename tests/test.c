#include <stdbool.h>
#include <stdio.h>

#include <libcsv.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s [file.csv]\n", argv[0]);
    return 1;
  }

  /************ csv_import() ************/

  CSV_METADATA *metadata = NULL;

  CSV_LIST *csv_list = csv_import(argv[1], &metadata);

  printf("\n%s has %d fields.\n", argv[1], metadata->fields);

  for (int i = 0; i < metadata->fields; i++) {
    printf("| %10s |", csv_list->field_list[i]->field);
  }

  printf("\n");

  unsigned int row = 0;
  unsigned int field = 0;
  bool last_row = false;

  while (1) {
    for (int i = 0; i < metadata->fields; i++) {
      unsigned current_row = 0;
      CSV_STRING_BLOCK *block = csv_list->field_list[i]->string_block_head;

      while (block != NULL) {
        if (current_row == row) {
          printf("| %10s | ", block->data);
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
    printf("\n");

    if (last_row) {
      break;
    }
  }

  /************ csv_field() ************/

  printf("\nExtracting 'First name' field...\n");

  CSV_FIELD_LIST *first_field = csv_field("First name", csv_list, metadata);

  CSV_STRING_BLOCK *block = first_field->string_block_head;

  while (block != NULL) {
    printf("| %10s | ", block->data);

    block = block->next_block;
  }

  printf("\n");

  /************ csv_export() ************/

  csv_export(csv_list, NULL, metadata);

  /************ csv_column() ************/

  printf("\nExtracting first column...\n");

  CSV_FIELD_LIST *first_column = csv_column(0, csv_list, metadata);

  CSV_STRING_BLOCK *column = first_column->string_block_head;

  while (column != NULL) {
    printf("| %10s | ", column->data);

    column = column->next_block;
  }

  printf("\n");

  /************ csv_add_row() ************/

  printf("\nAdding data row...\n");

  csv_add_row("booker12, 9012, Rachel, Booker", csv_list, metadata);

  csv_export(csv_list, NULL, metadata);

  /************ csv_remove_row() ************/

  printf("\nRemoving data row...\n");

  csv_remove_row(2, csv_list, metadata);

  csv_export(csv_list, NULL, metadata);

  return 0;
}