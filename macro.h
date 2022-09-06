
#ifndef MACRO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ctype.h>

#define MAX_LENGTH_OF_LINE1 80
#define MAX_LINES_OF_MACRO 10
#define SUCCESS_FLAG 0
#define ERROR_FLAG -1
#define MACRO "macro"
#define ENDMACRO "endmacro"

typedef struct { 
	char name[MAX_LENGTH_OF_LINE1];
	char content[MAX_LENGTH_OF_LINE1 * MAX_LINES_OF_MACRO];
	int line_of_macro;
} macro;

/*
Defining the struct of macro that includes its name, its content and the line it has been written, so that we could check if the macro has been defined before his declaration.
*/

int open_macro(char *);

/*
The program puts the macro statements into an array of macros, which is dynamically allocated.
Each element in the array is a struct that includes its name and content.
To get the words "macro" and "endmacro" and the name of the macro we used strtok function.
In order to check that we called the macro only after defining it, we saved the number of the line where the macro was defined.
*/


int create_file_am(char*,macro*,int);

/*
First we create the file.am then we read line by line and copy from the source file. 
If we come across name of a macro, we check if the call is not before the definition, and then we will write its content in the line corresponding to it in the file.am.
If we encountered a macro statement we will not copy these lines, and wait until we notice "endmacro".
*/

#endif
