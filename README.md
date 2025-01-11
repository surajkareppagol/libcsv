# 📊 libcsv

Yet another library for CSV files.

## ⚙️ Build

Build and run the example.

```sh
make
```

```sh
libcsv [options...]
```

```sh
libcsv -i users.csv -a "user,2002,admin,new" -p -e
```

## ➡️ Available Options

| Option | Description        | Arguments                         |
| ------ | ------------------ | --------------------------------- |
| i      | Import CSV         | CSV file name required            |
| a      | Append row of data | String of data separated by comma |
| r      | Remove row of data | Index number (0 - Max items)      |
| p      | Print CSV data     | None                              |
| e      | Export to CSV file | None                              |

## ⚙️ API

### 1. CSV_IMPORT

Pass csv filename and the address of metadata structure. If file found it will
extract the data and also fill up the metadata structure. Returns the pointer
to the csv list.

```c
CSV_LIST *csv_import(char *csv_file, CSV_METADATA **metadata);
```

### 2. CSV_EXPORT

Pass csv list and optional csv filename, also the metadata structure. A csv
file will be created and the data from csv list will be populated.

```c
void csv_export(CSV_LIST *csv_list, char *csv_file, CSV_METADATA *metadata);
```

### 3. CSV_FIELD

Pass field string, csv list and metadata structure. If field found it will
return the pointer to that block.

```c
void *csv_field(char *field, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

### 4. CSV_COLUMN

Similar to `csv_field()`, but accepts column number (starts from 0).

```c
void *csv_column(unsigned int column, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

### 5. CSV_ADD_ROW

Add new row of data to csv list.

```c
void csv_add_row(char *data, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

### 6. CSV_REMOVE_ROW

Remove a row of data from csv list.

```c
void csv_remove_row(unsigned int row, CSV_LIST *csv_list, CSV_METADATA *metadata);
```

### 7. CSV_SHOW

Display CSV data on to the terminal.

```c
void csv_show(CSV_LIST *csv_list, CSV_METADATA *metadata);
```
