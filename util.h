#ifndef _UTIL_H_
#define _UTIL_H_

/*
 * enum Type is intended to be used to communicate the basic data type of the
 * column data being read.
 */
enum Type {
    UNKNOWN, STRING, FLOAT, LONG, INT, CHAR
};
typedef enum Type Type_t;

/*
 * Returns the Type_t of the data under the column with the name given. If the
 * column name is not recognized, UNKNOWN is returned.
 */
Type_t column_type(char *column);

/*
 * Returns 1 if data matches the type specified by the column, and by extension
 * the basic data type of the column. Returns 0 otherwise.
 *
 * matches_type allocates a constant, non-increasing amount of memory once for
 * each column that is left behind after it returns. To free this memory, call
 * util_cleanup(). Note that freeing this memory will make the next call to
 * matches_type take slightly longer.
 */
int matches_type(char *column, char *data);

/*
 * Returns the default value as a c-string for the column given, or a null
 * pointer if there is no default. The return does not need to be freed.
 */
const char *get_default(char *column);

/*
 * Cleans up all memory allocated while checking data. This function is always
 * safe to call, regardless of whether or not it there is memory to be freed.
 */
void util_cleanup();

#endif /* _UTIL_H_ */