# 📊 libcsv

Yet another library for CSV files.

## ⚙️ Build

Build and run the example.

```sh
make
```

```sh
libcsv -i user.csv -a "a, b, c" -e
```

- `-i`, Import CSV data into a C Object
- `-a`, Append data to CSV
- `-e`, Export C Object to CSV data

## ⚙️ API

- Pass csv filename and the address of metadata structure. If file found it will
  extract the data and also fill up the metadata structure. Returns the pointer
  to the csv list.

```c
CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata);
```

---

- Pass csv list and optional csv filename, also the metadata structure. A csv
  file will be created and the data from csv list will be populated.

```c
void csv_export(CSV_LIST *csv_list, char *csv_file, CSV_METADATA *metadata);
```

---

- Pass field string, csv list and metadata structure. If field found it will
  return the pointer to that block.

```c
CSV_FIELD_LIST *csv_field(char *field, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

---

- Similar to `csv_field()`, but accepts column number (starts from 0).

```c
CSV_FIELD_LIST *csv_column(unsigned int column, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

---

- Add new row of data to csv list.

```c
void csv_add_row(char **data, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

---

- Remove a row of data from csv list.

```c
void csv_remove_row(unsigned int row, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

---

- Display CSV data on to the terminal.

```c
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata);
```

## 📦️ CSV Structures

`libcsv` has 4 objects which define how the `CSV` data is stored in **C**.

`csv_list` is the top level structure returned from `csv_import()` function.

```c
typedef struct csv_list {
  struct csv_field_list *field_list[CSV_MAX_FIELDS];

} CSV_LIST;
```

`csv_field_list` is the structure that holds fields and the pointers to data
blocks.

```c
typedef struct csv_field_list {
  char *field;

  struct csv_string_block *string_block_head;
  struct csv_numeric_block *numeric_block_head;

  struct csv_string_block *string_block_tail;
  struct csv_numeric_block *numeric_block_tail;
} CSV_FIELD_LIST;
```

These are `csv_string_block` and `csv_numeric_block`.

```c
typedef struct csv_string_block {
  struct csv_string_block *next_block;

  char *data;
} CSV_STRING_BLOCK;
```

```c
typedef struct csv_numeric_block {
  struct csv_numeric_block *next_block;

  int data;
} CSV_NUMERIC_BLOCK;
```
