//
//  main.c
//  analyze
//
//  Created by Matt Krueger on 10/22/15.
//  Copyright © 2015 Matt Krueger. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weatherio.h"
#include "query.h"

int main(int argc, const char * argv[]) {
    // Error if there are more than 3 command line arguments
    if (argc > 4) {
        printf("ERROR: Too many arguments! (Valid arguments are df, if, of)");
        return 0;
    }
    
    /*
     * These variables equal the index of each optional command line parameters of argv
     * they are -1 if the user doesn't give the parameter
     */
    int arg_num_df = -1;
    int arg_num_if = -1;
    int arg_num_of = -1;
    
    // Read command line parameters
    for (int i=1; i<argc; i++) {
        
        // check for df
        if (argv[i][0] == 'd' && argv[i][1] == 'f' && argv[i][2] == '=') {
            arg_num_df = i;
            printf("%s\n", argv[i]);
        }
        
        // check for if
        if (argv[i][0] == 'i' && argv[i][1] == 'f' && argv[i][2] == '=') {
            arg_num_if = i;
        }
        
        // check for of
        if (argv[i][0] == 'o' && argv[i][1] == 'f' && argv[i][2] == '=') {
            arg_num_of = i;
        }
    }
    
    char * data_filename = (char *)calloc(150, sizeof(char));
    
    if (arg_num_df != -1) {
        int filename_length = ((int)strlen(argv[arg_num_df])) - 3;
        //data_filename = (char *)malloc(filename_length * sizeof(char));
        //trimmed_data_filename is the command line argument string without "df="
        data_filename = (char *)realloc(data_filename, (filename_length+1) * sizeof(char));
        data_filename[filename_length] = '\0';
        for (int i=0; i<filename_length; i++) {
            char* c = argv[arg_num_df];
            char d = c[i+3];
            data_filename[i] = d;
        }
    } else {
        printf("Enter data filename\n");
        scanf("%s", data_filename);
    }

    int is_data_valid = validate_format(data_filename); // this function gives strange error when given invalid file, but I am using the provided hw2 object files?
    if(is_data_valid != 0) {
        return 0;
    }
    
    FILE * input_file = NULL;
    
    FILE * output_file = NULL;
    
    // If input file is specified, try to open it
    if (arg_num_if != -1) {
        
        // remove if= from command line parameter
        char * input_filename = (char *)calloc(150, sizeof(char));
        int filename_length = ((int)strlen(argv[arg_num_if])) - 3;
        input_filename = (char *)realloc(input_filename, (filename_length+1) * sizeof(char));
        input_filename[filename_length] = '\0';
        for (int i=0; i<filename_length; i++) {
            char* c = argv[arg_num_if];
            char d = c[i+3];
            input_filename[i] = d;
        }

        // open input file, verify
        input_file = fopen(input_filename, "r");
        if (input_file == NULL) {
            printf("ERROR: invalid input filename\n");
            return 0;
        }
    }

    
    // If output file is specified, try to open it
    if (arg_num_of != -1) {
        
        // remove of= from command line parameter
        char * output_filename = (char *)calloc(150, sizeof(char));
        int filename_length = ((int)strlen(argv[arg_num_of])) - 3;
        output_filename = (char *)realloc(output_filename, (filename_length+1) * sizeof(char));
        output_filename[filename_length] = '\0';
        for (int i=0; i<filename_length; i++) {
            char* c = argv[arg_num_of];
            char d = c[i+3];
            output_filename[i] = d;
        }
        // open and verify file
        output_file = fopen(output_filename, "w");
        if (output_file == NULL) {
            printf("ERROR: invalid output filename\n");
            return 0;
        }
    }
    
    // Execute queries
    if (arg_num_if != -1) {
        query_from_file(input_file, output_file, data_filename);
    } else {
        query_from_console(output_file, data_filename);
    }
    
    // close open files
    if (input_file != NULL) {
        fclose(input_file);
    }
    if (output_file != NULL) {
        fclose(output_file);
    }
    
    return 0;
}
