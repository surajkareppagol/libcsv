/**
 * @file util.c
 * @author Suraj Kareppagol (surajkareppagol.dev@gmail.com)
 * @brief Utility functions for libcsv
 *
 * @version 0.1
 * @date 2025-01-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libcsv.h>
#include <util.h>

/************************************************/
/*             UTIL_TRIM_STRING                 */
/************************************************/

char *util_trim_string(char *string) {
  int string_len = strlen(string);

  int begin = 0;
  int end = string_len - 1;

  char *in_string = string;

  /* -- Remove front spaces */
  while (in_string) {
    if (!isspace(*in_string)) {
      break;
    }

    in_string++;
    begin++;
  }

  /* -- Remove back spaces */
  while (string_len > -1) {
    if (!isspace(*(in_string - begin + end))) {
      break;
    }

    string_len--;
    end--;
  }

  /* -- Create new string */
  char *trimmed_string = calloc(1, end - begin + 2);

  int i = 0;

  for (i = 0; i < end - begin + 1; i++) {
    trimmed_string[i] = *in_string;

    in_string++;
  }

  trimmed_string[i] = '\0';

  return trimmed_string;
}

/************************************************/
/*             UTIL_STRING_TO_NUMBER            */
/************************************************/

int util_string_to_number(char *string, int *data) {
  char *characters;

  *data = strtol(string, &characters, 10);

  if (*characters != '\0') {
    return -1;
  }

  return 0;
}

/************************************************/
/*             UTIL_STRING_TO_DOUBLE            */
/************************************************/

int util_string_to_double(char *string, double *data) {
  if (strstr(string, ".") == NULL) {
    return -1;
  }

  char *characters;

  *data = strtod(string, &characters);

  if (*characters != '\0') {
    return -1;
  }

  return 0;
}

/************************************************/
/*             UTIL_TOTAL_FIELDS                */
/************************************************/

int util_total_fields(char *string) {
  char *buffer = calloc(1, strlen(string));
  strcpy(buffer, string);

  char *token = strtok(buffer, CSV_DELIMETER);

  unsigned total_fields = 0;

  while (token != NULL) {
    total_fields += 1;

    token = strtok(NULL, CSV_DELIMETER);
  }

  free(buffer);

  return total_fields;
}