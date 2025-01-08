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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <libcsv.h>

#define LIBCSV_ARGS "i:a:e"

int main(int argc, char **argv) {
  int opt;

  CSV_METADATA *metadata = NULL;
  CSV_LIST *csv_list = NULL;

  while ((opt = getopt(argc, argv, LIBCSV_ARGS)) != -1) {
    switch (opt) {
    case 'i':
      csv_list = csv_import(optarg, &metadata);
      csv_show(csv_list, metadata);
      break;
    case 'a':
      csv_add_row(optarg, csv_list, metadata);
      break;
    case 'e':
      csv_export(csv_list, NULL, metadata);
      break;
    default:
      fprintf(stderr,
              "Usage: %s -i [file] -a [data] -e"
              "\n-i = Import CSV data into C object"
              "\n-a = Append a row of data"
              "\n-e = Export C object into CSV file"
              "\n",
              argv[0]);
    }
  }
}