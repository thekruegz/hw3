//
//  query.h
//  analyze
//
//  Created by Matt Krueger on 10/23/15.
//  Copyright © 2015 Matt Krueger. All rights reserved.
//

#ifndef query_h
#define query_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "weatherio.h"

int query_from_file(FILE * query_file, FILE * out_file, char * data_filename);

int query_from_console(FILE * out_file, char * data_filename);

#endif /* query_h */
