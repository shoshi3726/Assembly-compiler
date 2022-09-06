
#ifndef TRANSITION1_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <ctype.h>

#define OP_CODE_LENGTH 16
#define REGISTERS_LENGTH 8
#define MAX_SIGN_LENGTH 9
#define MAX_LENGTH_OF_LINE 82
#define VALID_MAX_LENGTH_OF_LINE 80
#define BINARY_CODE_LENGTH 11
#define ERROR_FLAG -1
#define COMMA ','
#define SUCCESS_FLAG 0
#define COMMENT ';'


/*
Defining enum so that we can mark errors, and accordingly decide whether to continue moving forward.
*/

typedef enum {
   False, True
} boolean;


/*
Defining the struct of an instruction that includes the IC and the its binary code, and pointer to the next instruction of the instructions' linked list.
*/
typedef struct instruction
{
	int ic;
	char binary_code[BINARY_CODE_LENGTH];
	struct instruction* next;
}instruction;


/*
Defining the struct of data that includes the DC and the its binary code, and pointer to the next data of the datas' linked list.
*/
typedef struct data
{
	int dc;
	char binary_code[BINARY_CODE_LENGTH];
	struct data* next;
}data;


/*
Defining the struct of lable that includes its name, its adsress, its sign either "data", "string", "struct", "external", "entry", and pointer to the next lable of the lables' linked list.
*/
typedef struct lable { 
	char name[31];
	int adress;
	char sign[MAX_SIGN_LENGTH];
	struct lable* next;
} lable;


/*
Defining the struct of undefined lable that we didn't know its binary code at the first pass. It includes the IC , the lable nane, the pointer to the corresponding member of the instructions' linked list , and pointer to the next undefined lable of the undefined lables' linked list.
*/
typedef struct undefined_lable
{
	int ic;
	char lable_name[30];
	int line_number;
	instruction* instruction;
	struct undefined_lable* next;
} undefined_lable;



/*
Defines struct for the commands, which includes their name, and the function they point to.
*/
typedef struct {
char *name;
int (*func)(int,char*, instruction*, int,undefined_lable * );
} op_code;



/*
In the three add functions we add each new member to its belonged linked list, and initilize his next member to NULL.
*/		

/*Get a pointer to lable and try to allocate next lable.
If succed return False
Else
return True*/
int add_lable(lable ** );

/*Get a pointer to data and try to allocate next data.
If succed return False
Else
return True*/
int add_data(data ** );

/*Get a pointer to instruction and try to allocate next instruction.
If succed return False
Else
return True*/
int add_instruction(instruction ** );


/*
In the three free functions we release each dynamically allocated member of each linked list.		
*/
int free_all_instructions(instruction *);
int free_all_data(data *);	
int free_all_lables(lable *);
int free_all_undefined_lables(undefined_lable * );


/*
In the second pass we check if the label is defined as entry. If the label is already marked as "External" we will mark an error, if not, it will be marked as "Entry".
And we will go over all the labels that we didn't know what their coding was in the first pass, and we will now fill in their coding.
Get name of file, head to instruction,data and undefined_lable
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int second_pass(char* ,instruction * , data* ,lable* ,undefined_lable* );


/*
In the first pass, we separate each instruction into the number of operations it consumes, and assign them the locations and coding.
In case that we do not know the coding, we will mark "?", and we will save in struct that we defined in advance, the name of the label that we do not know how to code, and its IC number, so that in the second pass we can know where to complete the coding.
Using strtok with ":" we checked if the first word is a label.
If it was a label, we will insert its name and IC number into a struct we defined in advance, and add it to the linked list of labels.
In case there was a label, the next word is checked, and in case there was no label, the first word is checked.
If the first word is a directive sentence, in order to encode it, we will send it to a method that will decode it according to its definition; .data -> read_data, .string -> read_string, .struct -> read_struct.
The directive .extern will be decoded inside the function. The label that is written after it will be entered into the list of labels and marked as "external", if the label is already in the list of labels, it will be an error.
If the first word does not indicate a directive, it is probably the beginning of an instruction sentence (unless there is no such command, and this is an error)
Using a struct of an instruction name and a pointer to a function, we will send each instruction to its corresponding function.

Get name of file
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int first_transition(char*);


/*
All the methods that call the opcodes use the "find_opcode_1_operands" and "find_opcode_2_operands" functions according to the number of arguments they require.
After using the above functions, we check in the coding line of the opcode whether the types of operands we coded are indeed suitable for the current function.
The functions that do not accept arguments, only check that no data has been entered after them.
Get number of line, line, head to instruction,IC and undefined_lable
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/

int read_mov(int,char*, instruction*, int,undefined_lable * );
int read_cmp(int,char*, instruction*, int,undefined_lable * );
int read_add(int,char*, instruction*, int,undefined_lable * );
int read_sub(int,char*, instruction*, int,undefined_lable * );
int read_not(int,char*, instruction*, int,undefined_lable * );
int read_clr(int,char*, instruction*, int,undefined_lable * );
int read_lea(int,char*, instruction*, int,undefined_lable * );
int read_inc(int,char*, instruction*, int,undefined_lable * );
int read_dec(int,char*, instruction*, int,undefined_lable * );
int read_jmp(int,char*, instruction*, int,undefined_lable * );
int read_bne(int,char*, instruction*, int,undefined_lable * );
int read_get(int,char*, instruction*, int,undefined_lable * );
int read_prn(int,char*, instruction*, int,undefined_lable * );
int read_jsr(int,char*, instruction*, int,undefined_lable * );
int read_rts(int,char*, instruction*, int,undefined_lable * );
int read_hlt(int,char*, instruction*, int,undefined_lable * );


/*
The function receives a string of length five which is the left/right half of the binary coding of each line in the program, and converts the received string to a base 32 number, which is returned as a character.
Get a string which represent a binary code,
Returns char which represent its value in 32 base
*/
char convert_to_32base(char *);


/*
The function produces the ob. file, in which all the numbers of the addresses caught in the memory by the program are written, and the binary coding found in each address.
The addresses and encodings are represented in base 32.
In addition to this, the function creates an ext. file into which it inserts a label marked with "external", and an ent. file into which it inserts labels marked with "entry". The labels and their encoding are represented in base 32.
In addition, we kept a counter for each file, which counted the number of times a label was inserted into the file, so that we could check if the file is empty, and remove it.
get name of file, IC, DC, head of instructions list, head of data list,
head of lable list, and head of undefined_lable list.
returns ERROR_FLAG if failed creating file. else, returns SUCCESS_FLAG.
*/
int create_files(char *,int, int, instruction * , data *, lable *,undefined_lable*);


/*
The method reads the numbers after the label, and checks whether there are no unnecessary commas, and that indeed all the entered data are numbers.
Get number of line, line, head to data and DC
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int read_numbers(int,char*, data*,int );


/*
The method reads the string after the label, and encodes each character in a new encoding line. and checks whether a proper string has been entered.
Get number of line, line, head to data and DC
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int read_string(int,char*, data*,int );


/*
The method uses the read_data method to check if the first member of the struct is a number, and the read_string method to check if the second member of the struct is a string.
Get number of line, line, head to data and DC
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int read_struct(int,char*, data*,int );

/*
The function interprets the opcodes which have 1 argument, and codes the operand according to its address type in the coding line of the opcode, in the destination operand.The source operand is remain empty (with 00).
Get number of line, line, head to instruction, head to undedfined lable and IC
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int find_opcode_1_operands(int,char*, instruction*, int, undefined_lable * );


/*
The function interprets the opcodes which have 2 arguments, and codes each operand according to its address type in the coding line of the opcode.
Get number of line, line, head to instruction, head to undedfined lable and IC
return SUCCESS_FLAG if no errors found, 
else,
return ERROR_FLAG
*/
int find_opcode_2_operands(int,char*, instruction*, int, undefined_lable * );




#endif

