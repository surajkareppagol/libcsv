/**
 * @file main.c
 * @author Suraj Kareppagol (surajkareppagol.dev@gmail.com)
 * @brief libcsv executable entry
 * @version 0.1
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libcsv.h>
#include <util.h>

#define LIBCSV_ARGS "i:o:a:r:ph"

void csv_print_help(char *binary) {
  fprintf(stderr,
          "Usage: %s -i [file] -a [data] -e"
          "\n-i = Import CSV data into C object"
          "\n-o = Export C object into CSV file"
          "\n-a = Append a row of data"
          "\n-r = Remove a row of data"
          "\n-p = Print data"
          "\n-h = Help"
          "\n",
          binary);
}

int main(int argc, char **argv) {
  int opt;

  CSV_METADATA *metadata = NULL;
  CSV_LIST *csv_list = NULL;

  while ((opt = getopt(argc, argv, LIBCSV_ARGS)) != -1) {
    switch (opt) {
    case 'i': {
      csv_list = csv_import(optarg, &metadata);
      break;
    }
    case 'a': {
      csv_add_row(optarg, csv_list, metadata);
      break;
    }
    case 'r': {
      int row = 0;
      if ((util_string_to_number(optarg, &row) == 0) &&
          (row < metadata->items)) {
        csv_remove_row(row, csv_list, metadata);
        break;
      }

      fprintf(stderr,
              "Error: Invalid argument %s for"
              " option -r.\n",
              optarg);

      break;
    }
    case 'o': {
      csv_export(csv_list, metadata, optarg);
      break;
    }
    case 'p': {
      csv_show(csv_list, metadata);
      break;
    }
    case 'h': {
      csv_print_help(argv[0]);
      return 0;
    }
    default: {
      csv_print_help(argv[0]);
    }
    }
  }

  csv_clear(csv_list, metadata);

  return 0;
}
