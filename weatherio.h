#include <stdio.h>

#ifndef _WEATHERIO_H_
#define _WEATHERIO_H_

/* There was an error opening the file */
#define WEATHERIO_FILE_OPEN_ERR -1

/* The file opened, but there was an error reading its contents */
#define WEATHERIO_FILE_READ_ERR -2

/* A field exceeded the maximum expected length while being read */
#define WEATHERIO_MEMORY_FILLED -3

/* The type of the field did not match the expected type of its column */
#define WEATHERIO_TYPE_MISMATCH -4

/* There was an extra column in the header or data */
#define WEATHERIO_EXTRA_COLUMNS -5

/* There was a missing column in the data */
#define WEATHERIO_FEWER_COLUMNS -6

/* A column in the header showed up at least twice */
#define WEATHERIO_DOUBLE_COLUMN -7

/* A field was left empty for which no default is defined */
#define WEATHERIO_EMPTY_DEFAULT -8

/* An unrecognized column appeared in the header */
#define WEATHERIO_UNDEFINED_COL -9

/*
 * Opens, reads, and validates the headers and each field in the CSV pointed
 * to by filename. If the CSV is valid, it returns 0. Otherwise, it returns
 * either the first line it found an error (a positive value), or the error
 * code it reached while opening the file (a negative value). If there is an
 * error, it prints its description to stdout.
 *
 * Cleans up all memory after it returns.
 */
int validate_format(char *filename);

/*
 * Counts the number of columns in the CSV located in the specified file. The
 * return value should never be less than 1 unless it is an error code.
 */
int header_columns(char *filename);

/*
 * Opens the file specified and fills columns with each column name that it
 * finds. For each field it finds, read_header allocates enough memory for it
 * and sets each char * within columns to point to its location in memory.
 * It will terminate columns with a null pointer.
 *
 * The calling function is in charge of freeing each member of columns when it
 * no longer needs it.
 */
int read_header(char *filename, char **columns);

/*
 * Reads the row of data pointed to by file. columns is expected to have the
 * names of each of the columns in the file, and data will be filled out with
 * the data unless there is an error while reading the row. If there is an
 * error, elements in data will be freed and point to garbage. read_row will
 * continue reading fields into data until it sees a null pointer in columns,
 * at which point it will return WEATHERIO_EXTRA_COLUMNS if there are still
 * unread fields.
 *
 * The return value is either 0 (success) or (error_code << 16) | field (on
 * failure). If the return is nonzero, then the following code snippet will
 * give the error code:
 * (result >> 16) | 0xFFFF0000
 *
 * The following snippet gives the field number in which the error occurred:
 * result & 0xFFFF
 *
 * Upon returning, the calling function is in charge of freeing each member
 * of data when it is no longer needed.
 *
 * read_row allocates a constant, non-increasing amount of memory one time
 * that is left behind after it returns. To free this memory, call
 * weatherio_cleanup(). Note that freeing this memory will make the next call
 * to read_row take slightly longer.
 */
int read_row(FILE *file, char **columns, void **data);

/*
 * Returns the c-string with the description of the error values shown above.
 * The c-string does not need to be freed.
 */
const char *error_message(int error);

/*
 * Frees all data allocated by read_row if it exists. This function is safe to
 * call regardless of whether or not read_row has been called.
 */
void weatherio_cleanup();

#endif /* _WEATHERIO_H_ */
