#ifndef LIBCSV
#define LIBCSV

#define CSV_BUFFER_SIZE 1024
#define CSV_DELIMETER ","

#define CSV_MAX_FIELDS 1024

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

  struct csv_string_block *string_block;
  struct csv_numeric_block *numeric_block;
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

void csv_field();

void csv_row();
void csv_column();

void csv_add_data();
void csv_remove_data();

CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata);
void csv_export();

#endif