# 📊 libcsv

Yet another library for CSV files. It is a single header file `libcsv.h`.

## ⚙️ Build

Build and run the example.

```sh
make
```

```sh
make build
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

  struct csv_string_block *string_block;
  struct csv_numeric_block *numeric_block;
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
