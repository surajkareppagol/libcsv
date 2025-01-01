#include <stdio.h>

#include <libcsv.h>

int main(int argc, char **argv) {
  printf("libcsv - Yet another CSV library\n");

  CSV_METADATA *metadata = NULL;

  CSV_LIST *csv_list = csv_import(argv[1], &metadata);

  printf("\n%s has %d fields.\n", argv[1], metadata->fields);

  for (int i = 0; i < metadata->fields; i++) {
    printf("| %10s |", csv_list->field_list[i]->field);
  }

  printf("\n");

  for (int i = 0; i < metadata->fields; i++) {
    CSV_STRING_BLOCK *block = csv_list->field_list[i]->string_block;

    while (block != NULL) {
      printf("| %10s | ", block->data);

      block = block->next_block;
    }

    printf("\n");
  }

  return 0;
}