//
//  query.c
//  analyze
//
//  Created by Matt Krueger on 10/23/15.
//  Copyright © 2015 Matt Krueger. All rights reserved.
//

#include "query.h"

int get_month(char * date_string) {
    char * m;
    m = calloc(3, sizeof(char));
    m[0] = date_string[0];
    m[1] =date_string[1];
    int month = atoi(m);
    return month;
}

int get_day(char * date_string) {
    char * d;
    d = calloc(3, sizeof(char));
    d[0] = date_string[3];
    d[1] =date_string[4];
    int day = atoi(d);
    return day;
}

int get_year(char * date_string) {
    char * y;
    y = calloc(5, sizeof(char));
    y[0] = date_string[6];
    y[1] =date_string[7];
    y[2] = date_string[8];
    y[3] =date_string[9];
    int year = atoi(y);
    return year;
}

int get_hour(char * date_string) {
    char * h;
    h = calloc(3, sizeof(char));
    h[0] = date_string[11];
    h[1] =date_string[12];
    int hour = atoi(h);
    return hour;
}

int get_minute(char * date_string) {
    char * m;
    m = calloc(3, sizeof(char));
    m[0] = date_string[14];
    m[1] =date_string[15];
    int minute = atoi(m);
    return minute;
}

// 0 for no, 1 for yes
int is_in_range(char * start_date_string, char * end_date_string, long * date_column, int * time_column) {
    
    int start_month = get_month(start_date_string);
    int start_day = get_day(start_date_string);
    int start_year = get_year(start_date_string);
    int start_hour = get_hour(start_date_string);
    int start_minute = get_minute(start_date_string);
    
    int end_month = get_month(end_date_string);
    int end_day = get_day(end_date_string);
    int end_year = get_year(end_date_string);
    int end_hour = get_hour(end_date_string);
    int end_minute = get_minute(end_date_string);
    
    int row_year = ((int)*date_column) / 10000;
    int row_month = ((int)*date_column) / 100;
    row_month = row_month % 100;
    int row_day =((int)*date_column) % 100;
    
    if (*time_column > 2359 || *time_column < 0) {
        return -1;
    }
    
    int row_hour = *time_column / 100;
    int row_minute = *time_column % 100;

    if (start_year > end_year) {
        return -1;
    }
    if (start_year == end_year && start_month > end_month) {
        return -1;
    }
    if (start_year == end_year && start_month == end_month && start_day > end_day) {
        return -1;
    }
    if (start_year == end_year && start_month == end_month && start_day == end_day && start_hour > end_hour) {
        return -1;
    }
    if (start_year == end_year && start_month == end_month && start_day == end_day && start_hour == end_hour && start_minute > end_minute) {
        return -1;
    }
    
    // check if measurement was taken after start time, if not return 0
    if (row_year >= start_year) {
        if (row_year == start_year) {
            if (row_month >= start_month) {
                if (row_month == start_month) {
                    if (row_day >= start_day) {
                        if (row_day == start_day) {
                            if (row_hour >= start_hour) {
                                if (row_hour == start_hour) {
                                    if (row_minute >= start_minute) {
                                    } else return 0;
                                }
                            } else return 0;
                        }
                    } else return 0;
                }
            } else return 0;
        }
    } else return 0;
    
    if (row_year <= end_year) {
        if (row_year == end_year) {
            if(row_month <= end_month) {
                if (row_month == end_month) {
                    if (row_day <= end_day) {
                        if (row_day == end_day) {
                            if (row_hour <= end_hour) {
                                if (row_hour == end_hour) {
                                    if (row_minute <= end_minute) {
                                    } else return 0;
                                }
                            } else return 0;
                        }
                    } else return 0;
                }
            } else return 0;
        }
    } else return 0;
    
    return 1;
}


int validate_histogram(char * histogram_query) {
    
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    int bins;
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(histogram_query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (histogram_query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 6) {
        sscanf(histogram_query, "%s %s %s %s %d %s %s", command, header, header_word_2, header_word_3, &bins, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 5) {
        sscanf(histogram_query, "%s %s %s %d %s %s", command, header, header_word_2, &bins, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(histogram_query, "%s %s %d %s %s", command, header, &bins, start_date, end_date);
    }
    printf("C: %s, H: %s, H2: %s, H3: %s, B:%d, S:%s, E:%s\n", command, header, header_word_2, header_word_3, bins, start_date, end_date);
    
    // check if <header> is valid. Must contain numerical values only. Must be a string.
    Type_t data_type;
    data_type = column_type(header);
    if (data_type == UNKNOWN || data_type == STRING || data_type == CHAR) {
        printf("ERROR invalid query: %s\nreason: %s is not valid", histogram_query, header);
        return 0;
    }
    
    // check if <bins> is valid. Must be an int.
    if (bins <= 0) {
        printf("ERROR invalid query: %s\nreason: cannot have zero or negative bins\n", histogram_query);
        return 0;
    }
    // check if <start date/time> and <end date/time> are valid. Must be mm/dd/yyyy-hh:mm eg: 02/22/1995-22:57
    if (strlen(start_date) != 16 || get_month(start_date) <= 0 || get_month(start_date) > 12 || get_day(start_date) <= 0 || get_day(start_date) > 31 || get_hour(start_date) < 0 || get_hour(start_date) > 23 || get_minute(start_date) < 0 || get_minute(start_date) > 59) {
        printf("ERROR invalid query: %s\nreason: %s is an invalid start date \n", histogram_query, start_date);
        return 0;
    }
    if (strlen(end_date) != 16 || get_month(end_date) <= 0 || get_month(end_date) > 12 || get_day(end_date) <= 0 || get_day(end_date) > 31 || get_hour(end_date) < 0 || get_hour(end_date) > 23 || get_minute(end_date) < 0 || get_minute(end_date) > 59) {
        printf("ERROR invalid query: %s\nreason: %s is an invalid end date \n", histogram_query, end_date);
        return 0;
    }
    
    return 1;
}

int validate_avg_min_or_max(char * query) {
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 5) {
        sscanf(query, "%s %s %s %s %s %s", command, header, header_word_2, header_word_3, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 4) {
        sscanf(query, "%s %s %s %s %s", command, header, header_word_2, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(query, "%s %s %s %s", command, header, start_date, end_date);
    }
    
    // check if <header> is valid. Must contain numerical values only. Must be a string.
    Type_t data_type;
    data_type = column_type(header);
    if (data_type == UNKNOWN || data_type == STRING || data_type == CHAR) {
        printf("ERROR invalid query: %s\nreason: %s is not valid", query, header);
        return 0;
    }
    
    // check if <start date/time> and <end date/time> are valid. Must be mm/dd/yyyy-hh:mm eg: 02/22/1995-22:57
    if (strlen(start_date) != 16 || get_month(start_date) <= 0 || get_month(start_date) > 12 || get_day(start_date) <= 0 || get_day(start_date) > 31 || get_hour(start_date) < 0 || get_hour(start_date) > 23 || get_minute(start_date) < 0 || get_minute(start_date) > 59) {
        printf("ERROR invalid query: %s\nreason: %s is an invalid start date \n", query, start_date);
        return 0;
    }
    if (strlen(end_date) != 16 || get_month(end_date) <= 0 || get_month(end_date) > 12 || get_day(end_date) <= 0 || get_day(end_date) > 31 || get_hour(end_date) < 0 || get_hour(end_date) > 23 || get_minute(end_date) < 0 || get_minute(end_date) > 59) {
        printf("ERROR invalid query: %s\nreason: %s is an invalid end date \n", query, end_date);
        return 0;
    }
    
    return 1;
}

void execute_histogram(char * histogram_query, char * data_filename, FILE * output) {
    
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    int bins;
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(histogram_query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (histogram_query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 6) {
        sscanf(histogram_query, "%s %s %s %s %d %s %s", command, header, header_word_2, header_word_3, &bins, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 5) {
        sscanf(histogram_query, "%s %s %s %d %s %s", command, header, header_word_2, &bins, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(histogram_query, "%s %s %d %s %s", command, header, &bins, start_date, end_date);
    }
    
    Type_t data_type;
    data_type = column_type(header);
    
    int num_data_columns = header_columns(data_filename);
    char * column_headers[num_data_columns];
    read_header(data_filename, column_headers);
    
    int data_column_index = -1;
    int date_index = -1;
    int time_index = -1;
    
    char  date[] = "Date";
    char time[] = "HrMn";
    
    //date is LONG time is INT
    
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], header) == 0) {
            data_column_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], date) == 0) {
            date_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], time) == 0) {
            time_index = i;
        }
    }
    
    FILE * data_file = NULL;
    data_file = fopen(data_filename, "r");
    if (data_file == NULL) {
        printf("ERROR invalid data file: %s/n", data_filename);
    }
    
    void * row_data[num_data_columns];
    
    int ivalue; int imax; int imin;
    long lvalue ; long lmax; long lmin;
    float fvalue; float fmax;  float fmin; float maximum; float minimum;
    int row_count = 0;
    
    long * data_date;
    //    data_date = calloc(9, sizeof(char));
    
    int * data_time;
    //    data_time = calloc(5, sizeof(char));
    
    
    char * row;
    row = (char *)calloc(200,sizeof(char));
    int status = 0;
    // read file for maximum
    fgets(row, 200, data_file);
    while(fscanf(data_file, "*") != EOF) {
        status = read_row(data_file, column_headers, row_data);
        data_date = ((long*)row_data[date_index]);
        data_time = ((int*)row_data[time_index]);
        
        
        int in_time_range = is_in_range(start_date, end_date, data_date, data_time);
        if (in_time_range == 1) {
            if (data_type==FLOAT) {
                fvalue = *((float*)row_data[data_column_index]);
                if(row_count == 0) {
                    fmax = fvalue;
                } else if (fvalue > fmax) {
                    fmax = fvalue;
                }
            }
            if (data_type==INT) {
                ivalue = *((int*)row_data[data_column_index]);
                if(row_count == 0) {
                    imax = ivalue;
                } else if (ivalue > imax) {
                    imax = ivalue;
                }
            }
            if (data_type==LONG) {
                lvalue = *((long*)row_data[data_column_index]);
                if(row_count == 0) {
                    lmax = lvalue;
                } else if (lvalue > lmax) {
                    lmax = lvalue;
                }
            }
        }
        row_count++;
    }
    if (data_type==FLOAT) {
        printf("Maximum = %f\n",fmax);
        maximum = fmax;
    }else if (data_type==INT) {
        printf("Maximum = %d\n",imax);
        maximum = (float)imax;
    } else if (data_type==LONG) {
        printf("Maximum = %ld\n",lmax);
        maximum = (float)lmax;
    }
    
    rewind(data_file);
    
    fgets(row, 200, data_file);
    while(fscanf(data_file, "*") != EOF) {
        status = read_row(data_file, column_headers, row_data);
        data_date = ((long*)row_data[date_index]);
        data_time = ((int*)row_data[time_index]);
        
        
        int in_time_range = is_in_range(start_date, end_date, data_date, data_time);
        if (in_time_range == 1) {
            if (data_type==FLOAT) {
                fvalue = *((float*)row_data[data_column_index]);
                if(row_count == 0) {
                    fmin = fvalue;
                } else if (fvalue < fmin) {
                    fmin = fvalue;
                }
            }
            if (data_type==INT) {
                ivalue = *((int*)row_data[data_column_index]);
                if(row_count == 0) {
                    imin = ivalue;
                } else if (ivalue < imin) {
                    imin = ivalue;
                }
            }
            if (data_type==LONG) {
                lvalue = *((long*)row_data[data_column_index]);
                if(row_count == 0) {
                    lmin = lvalue;
                } else if (lvalue < lmin) {
                    lmin = lvalue;
                }
            }
        }
        row_count++;
    }
    if (data_type==FLOAT) {
        printf("Minimum = %f\n",fmin);
        minimum = fmin;
    }else if (data_type==INT) {
        printf("Minimum = %d\n",imin);
        minimum = (float)imin;
    } else if (data_type==LONG) {
        printf("Minimum = %ld\n",lmin);
        minimum = (float)lmin;
    }

}

float execute_max(char * max_query, char * data_filename, FILE * output) {
    
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(max_query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (max_query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 5) {
        sscanf(max_query, "%s %s %s %s %s %s", command, header, header_word_2, header_word_3, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 4) {
        sscanf(max_query, "%s %s %s %s %s", command, header, header_word_2, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(max_query, "%s %s %s %s", command, header, start_date, end_date);
    }
    
    Type_t data_type;
    data_type = column_type(header);
    
    int num_data_columns = header_columns(data_filename);
    char * column_headers[num_data_columns];
    read_header(data_filename, column_headers);
   
    int data_column_index = -1;
    int date_index = -1;
    int time_index = -1;
    
    char  date[] = "Date";
    char time[] = "HrMn";

    //date is LONG time is INT

    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], header) == 0) {
            data_column_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], date) == 0) {
            date_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], time) == 0) {
            time_index = i;
        }
    }
    
    if (data_column_index == -1) {
        printf("ERROR header %s not found in data file\n Query: %s\n", header, max_query);
        return 0;
    }
    if (time_index == -1) {
        printf("ERROR header HrMn not found in data file\n Query: %s\n", max_query);
        return 0;
    }
    if (date_index == -1) {
        printf("ERROR header Date not found in data file\n Query: %s\n", max_query);
        return 0;
    }
    
    FILE * data_file = NULL;
    data_file = fopen(data_filename, "r");
    if (data_file == NULL) {
        printf("ERROR invalid data file: %s/n", data_filename);
    }
    
    void * row_data[num_data_columns];
    
    int ivalue = 0; int imax = 0;
    long lvalue = 0; long lmax = 0;
    float fvalue = 0; float fmax = 0; float maximum = 0;
    int row_count = 0;
    
    long * data_date;
//    data_date = calloc(9, sizeof(char));
    
    int * data_time;
//    data_time = calloc(5, sizeof(char));

    char * row;
    row = (char *)calloc(200,sizeof(char));
    int status = 0;
    // read file
    fgets(row, 200, data_file);
    while(fscanf(data_file, "*") != EOF) {
        status = read_row(data_file, column_headers, row_data);
        data_date = ((long*)row_data[date_index]);
        data_time = ((int*)row_data[time_index]);
        
        
        int in_time_range = is_in_range(start_date, end_date, data_date, data_time);
        if (in_time_range == 1) {
            if (data_type==FLOAT) {
                fvalue = *((float*)row_data[data_column_index]);
                if(row_count == 0) {
                    fmax = fvalue;
                } else if (fvalue > fmax) {
                    fmax = fvalue;
                }
            }
            if (data_type==INT) {
                ivalue = *((int*)row_data[data_column_index]);
                if(row_count == 0) {
                    imax = ivalue;
                } else if (ivalue > imax) {
                    imax = ivalue;
                }
            }
            if (data_type==LONG) {
                lvalue = *((long*)row_data[data_column_index]);
                if(row_count == 0) {
                    lmax = lvalue;
                } else if (lvalue > lmax) {
                    lmax = lvalue;
                }
            }
        }
        row_count++;
    }
    if (data_type==FLOAT) {
        if (output == NULL) {
            printf("Maximum = %f\n",fmax);
            maximum = fmax;
        } else {
            fprintf(output, "Maximum = %f\n",fmax);
        }
    }else if (data_type==INT) {
        if (output == NULL) {
            printf("Maximum = %d\n",imax);
            maximum = (float)imax;
        } else {
            fprintf(output, "Maximum = %d\n",imax);
        }

    } else if (data_type==LONG) {
        if (output == NULL) {
            printf("Maximum = %ld\n",lmax);
            maximum = (float)lmax;
        } else {
            fprintf(output, "Maximum = %ld\n",lmax);
        }
    }
    return maximum;
}

float execute_min(char * min_query, char * data_filename, FILE * output) {
    
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(min_query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (min_query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 5) {
        sscanf(min_query, "%s %s %s %s %s %s", command, header, header_word_2, header_word_3, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 4) {
        sscanf(min_query, "%s %s %s %s %s", command, header, header_word_2, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(min_query, "%s %s %s %s", command, header, start_date, end_date);
    }
    
    Type_t data_type;
    data_type = column_type(header);
    
    int num_data_columns = header_columns(data_filename);
    char * column_headers[num_data_columns];
    read_header(data_filename, column_headers);
    
    int data_column_index = -1;
    int date_index = -1;
    int time_index = -1;
    
    char  date[] = "Date";
    char time[] = "HrMn";
    
    //date is LONG time is INT
    
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], header) == 0) {
            data_column_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], date) == 0) {
            date_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], time) == 0) {
            time_index = i;
        }
    }
    
    if (data_column_index == -1) {
        printf("ERROR header %s not found in data file\n Query: %s\n", header, min_query);
        return 0;
    }
    if (time_index == -1) {
        printf("ERROR header HrMn not found in data file\n Query: %s\n", min_query);
        return 0;
    }
    if (date_index == -1) {
        printf("ERROR header Date not found in data file\n Query: %s\n", min_query);
        return 0;
    }
    
    FILE * data_file = NULL;
    data_file = fopen(data_filename, "r");
    if (data_file == NULL) {
        printf("ERROR invalid data file: %s/n", data_filename);
    }
    
    void * row_data[num_data_columns];
    
    int ivalue = 0; int imin = 0;
    long lvalue = 0; long lmin = 0;
    float fvalue = 0; float fmin = 0; float minimum = 0;
    int row_count = 0;
    
    long * data_date;
    //    data_date = calloc(9, sizeof(char));
    
    int * data_time;
    //    data_time = calloc(5, sizeof(char));
    
    char * row;
    row = (char *)calloc(200,sizeof(char));
    int status = 0;
    // read file
    fgets(row, 200, data_file);
    while(fscanf(data_file, "*") != EOF) {
        status = read_row(data_file, column_headers, row_data);
        data_date = ((long*)row_data[date_index]);
        data_time = ((int*)row_data[time_index]);
        
        
        int in_time_range = is_in_range(start_date, end_date, data_date, data_time);
        if (in_time_range == 1) {
            if (data_type==FLOAT) {
                fvalue = *((float*)row_data[data_column_index]);
                if(row_count == 0) {
                    fmin = fvalue;
                } else if (fvalue < fmin) {
                    fmin = fvalue;
                }
            }
            if (data_type==INT) {
                ivalue = *((int*)row_data[data_column_index]);
                if(row_count == 0) {
                    imin = ivalue;
                } else if (ivalue < imin) {
                    imin = ivalue;
                }
            }
            if (data_type==LONG) {
                lvalue = *((long*)row_data[data_column_index]);
                if(row_count == 0) {
                    lmin = lvalue;
                } else if (lvalue < lmin) {
                    lmin = lvalue;
                }
            }
        }
        row_count++;
    }
    if (data_type==FLOAT) {
        if (output == NULL) {
            printf("Minimum = %f\n",fmin);
            minimum = fmin;
        } else {
            fprintf(output, "Minimum = %f\n",fmin);
        }
    }else if (data_type==INT) {
        if (output == NULL) {
            printf("Minimum = %d\n",imin);
            minimum = (float)imin;
        } else {
            fprintf(output, "Minimum = %d\n",imin);
        }
    } else if (data_type==LONG) {
        if (output == NULL) {
            printf("Minimum = %ld\n",lmin);
             minimum = (float)lmin;
        } else {
            fprintf(output, "Minimum = %ld\n",lmin);
        }
    }
    return minimum;
}

float execute_avg(char * avg_query, char * data_filename, FILE * output) {
    
    char * command;
    command = (char *)calloc(11, sizeof(char));
    char * header;
    header = (char *)calloc(17, sizeof(char));
    char * header_word_2;
    header_word_2 = (char *)calloc(4, sizeof(char));
    char * header_word_3;
    header_word_3 = (char *)calloc(2, sizeof(char));
    char * start_date;
    start_date = (char *)calloc(17, sizeof(char));
    char * end_date;
    end_date = (char *)calloc(19, sizeof(char));
    
    // count whitespace in query to determine if header is multiple words
    int query_length = (int)strlen(avg_query);
    int num_spaces = 0;
    for (int i=0; i<query_length; i++) {
        if (avg_query[i] == ' ') {
            num_spaces++;
        }
    }
    if (num_spaces == 5) {
        sscanf(avg_query, "%s %s %s %s %s %s", command, header, header_word_2, header_word_3, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        header[i+k+1] = ' ';
                        header[i+k+2] = header_word_3 [0];
                        header[i+k+3] = '\0';
                        break;
                    }
                }
                break;
            }
        }
    } else if (num_spaces == 4) {
        sscanf(avg_query, "%s %s %s %s %s", command, header, header_word_2, start_date, end_date);
        for (int i=0; i<40; i++) {
            if (header[i] == '\0') {
                header[i] = ' ';
                for (int k=0; k<40; k++) {
                    if (header_word_2[k] != '\0') {
                        header[i+k+1] = header_word_2[k];
                    } else {
                        break;
                    }
                }
                break;
            }
        }
    } else {
        sscanf(avg_query, "%s %s %s %s", command, header, start_date, end_date);
    }
    
    Type_t data_type;
    data_type = column_type(header);
    
    int num_data_columns = header_columns(data_filename);
    char * column_headers[num_data_columns];
    read_header(data_filename, column_headers);
    
    int data_column_index = -1;
    int date_index = -1;
    int time_index = -1;
    
    char  date[] = "Date";
    char time[] = "HrMn";
    
    //date is LONG time is INT
    
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], header) == 0) {
            data_column_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], date) == 0) {
            date_index = i;
        }
    }
    for (int i = 0; i<num_data_columns; i++) {
        if (strcmp(column_headers[i], time) == 0) {
            time_index = i;
        }
    }
    
    if (data_column_index == -1) {
        printf("ERROR header %s not found in data file\n Query: %s\n", header, avg_query);
        return 0;
    }
    if (time_index == -1) {
        printf("ERROR header HrMn not found in data file\n Query: %s\n", avg_query);
        return 0;
    }
    if (date_index == -1) {
        printf("ERROR header Date not found in data file\n Query: %s\n", avg_query);
        return 0;
    }
    
    FILE * data_file = NULL;
    data_file = fopen(data_filename, "r");
    if (data_file == NULL) {
        printf("ERROR invalid data file: %s/n", data_filename);
    }
    
    void * row_data[num_data_columns];
    
    int ivalue = 0;
    long lvalue = 0;
    float fvalue = 0; float avg = 0;
    int data_count = 0;
    int row_count = 0;
    
    long * data_date;
    //    data_date = calloc(9, sizeof(char));
    
    int * data_time;
    //    data_time = calloc(5, sizeof(char));
    
    char * row;
    row = (char *)calloc(200,sizeof(char));
    int status = 0;
    // read file
    fgets(row, 200, data_file);
    while(fscanf(data_file, "*") != EOF) {
        status = read_row(data_file, column_headers, row_data);
        data_date = ((long*)row_data[date_index]);
        data_time = ((int*)row_data[time_index]);
        
        
        int in_time_range = is_in_range(start_date, end_date, data_date, data_time);
        if (in_time_range == 1) {
            data_count++;
            if (data_type==FLOAT) {
                fvalue = fvalue + *((float*)row_data[data_column_index]);
            }
            if (data_type==INT) {
                ivalue = ivalue + *((int*)row_data[data_column_index]);
            }
            if (data_type==LONG) {
                lvalue = lvalue + *((long*)row_data[data_column_index]);
            }
        }
        row_count++;
    }
    if (data_type==FLOAT) {
        if (output == NULL) {
            avg = fvalue/data_count;
            printf("Average = %f\n",avg);
        } else {
            fprintf(output, "Average = %f\n",avg);
        }
        
    }else if (data_type==INT) {
        if(output == NULL) {
            avg = (float) ivalue/data_count;
            printf("Average = %d\n",(int)avg);
        }else {
            fprintf(output, "Average = %d\n",(int)avg);
        }
    } else if (data_type==LONG) {
        if(output == NULL) {
            avg = (float) lvalue/data_count;
            printf("Average = %ld\n",(long)avg);
        }else {
            fprintf(output, "Average = %ld\n",(long)avg);
        }
    }
    return avg;
}

int query_from_file(FILE * query_file, FILE * out_file, char * data_filename) {
    
    char * query;
    query = (char *)calloc(100,sizeof(char));
    
    // read file
    while (fgets(query, 150, query_file) != NULL) {
        
        // append the \n character
        int query_length = (int)strlen(query);
        query[query_length] = '\0';
        
        // check first word for the type of query
        char * query_type;
        query_type = (char *)calloc(9,sizeof(char));
        sscanf(query, "%s", query_type);
        
        // verify query_type
        int is_histogram = strcmp(query_type, "histogram");
        int is_min = strcmp(query_type, "min");
        int is_max = strcmp(query_type, "max");
        int is_average = strcmp(query_type, "average");
        if (is_histogram !=0 && is_min !=0 && is_max !=0 && is_average !=0) {
            printf("ERROR executing query: %s \nreason: invalid command %s \n", query, query_type);
            return 0;
        } else {
            if (is_histogram == 0) {
                int is_valid = validate_histogram(query);
                if (is_valid == 1) {
                    execute_histogram(query, data_filename, out_file);
                }
            }
            if (is_max == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_max(query, data_filename, out_file);
            }
            if (is_min == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_min(query, data_filename, out_file);
            }
            if (is_average == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_avg(query, data_filename, out_file);
            }
        }
        
        // reset memory to hold query
        free(query);
        query = (char *)calloc(100,sizeof(char));
    }
    
    return 1;
}

int query_from_console(FILE * out_file, char * data_filename) {
    
    char * query;
    query = (char *)calloc(100,sizeof(char));
    
    // read file
    while (1) {
        printf("Enter query: \n");
        gets(query);
        // append the \n character
        int query_length = (int)strlen(query);
        query[query_length] = '\0';
        
        char q_string[] = "quit";
        if (strcmp(query, q_string) == 0) {
            break;
        }
        // check first word for the type of query
        char * query_type;
        query_type = (char *)calloc(9,sizeof(char));
        sscanf(query, "%s", query_type);
        
        // verify query_type
        int is_histogram = strcmp(query_type, "histogram");
        int is_min = strcmp(query_type, "min");
        int is_max = strcmp(query_type, "max");
        int is_average = strcmp(query_type, "average");
        if (is_histogram !=0 && is_min !=0 && is_max !=0 && is_average !=0) {
            printf("ERROR executing query: %s \nreason: invalid command %s \n", query, query_type);
            return 0;
        } else {
            if (is_histogram == 0) {
                int is_valid = validate_histogram(query);
                if (is_valid == 1) {
                    execute_histogram(query, data_filename, out_file);
                }
            }
            if (is_max == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_max(query, data_filename, out_file);
            }
            if (is_min == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_min(query, data_filename, out_file);
            }
            if (is_average == 0) {
                int is_valid = validate_avg_min_or_max(query);
                execute_avg(query, data_filename, out_file);
            }
        }
        
        // reset memory to hold query
        free(query);
        query = (char *)calloc(100,sizeof(char));
    }
    
    return 1;
}






