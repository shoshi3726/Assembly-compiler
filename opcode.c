
#include "transition1.h"

int find_opcode_2_operands(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	char registers[REGISTERS_LENGTH][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
	instruction * head_op_code = pointer;


	char words[2][MAX_LENGTH_OF_LINE];
	/*save the 2 operands after the opcode*/
	/*If it has already read a register in the first operand, we will have to code them in the same line of coding.*/
	char string_num[MAX_LENGTH_OF_LINE];
	boolean  was_register = False;
	char *token = NULL;
	char* seperate_word = ",";
	char original_line[MAX_LENGTH_OF_LINE];
	char * rest_line = NULL;
	int i=0,j, index = 4,k,index_words=0,num,counter = 0,start = 0;
	/*Cleaning white characters.
	If we have reached the end of the line, it means that no operands have been inserted, and this is an error.*/

	for(i=0;i<strlen(line) && isspace(line[i]);i++);
	if(i == strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. Excepted arguments.\n");
		return ERROR_FLAG;
	
	}
	start = i;
	for(k=i; k<strlen(line); k++)
	{
		if(line[k] == COMMA)
			counter += 1;
	}
	/*Suppose to get two arguments with seperating comma. Didnt find comma. Error*/
	if(counter == 0)
	{
		for(k = 0; k<strlen(line) && isspace(line[k]); k++);
		for(j = k; j<strlen(line) && !isspace(line[j]); j++);
		for(k = j; k<strlen(line) && isspace(line[k]); k++);
		if(k == strlen(line))
		{
			printf("In line: %d\t",line_number);
			printf("Error. Missing argument.\n");
		}
		else
		{
			printf("In line: %d\t",line_number);
			printf("Error. Missing comma.\n");
		}
		return ERROR_FLAG;
	}
	strcpy(original_line,line);
	token = strtok(line,seperate_word);/*The token is until the comma*/

	
	/*As long as we didn't reach the end of the line and read at most 2 words.*/
	while(token!=NULL && line!=NULL && index_words<2)
	{
		for(k=0;k<strlen(token) && isspace(token[k]);k++);/*Go over the white characters*/
		for(j=k;k<strlen(token) && !isspace(token[j]);j++);/*Find out the length of the word*/				
		strncpy(words[index_words],token+k,j-k);/*copy the word*/
		words[index_words][j-k] = '\0';
		for(k=j;k<strlen(token) && isspace(token[k]);k++);/*Go over the white characters*/
		/*If after the word we come across a character that is not white, it is an incorrect name.*/
		if(k<strlen(token))
		{
			printf("In line: %d\t",line_number);
			printf("Error. Invalid name of argument.\n");
			return ERROR_FLAG;
		}
		token = strtok(NULL,seperate_word);
		/*Go through the current operand and check if it is one of the registers*/
		for(i=0;i<REGISTERS_LENGTH && strcmp(words[index_words],registers[i])!=0;i++);
		if(i!=REGISTERS_LENGTH)/*Found register*/
		{
			if(was_register == False)/*If there was no register yet, we will encode a new coding line.*/
			{
				was_register = True;
				if( add_instruction(&pointer) == True)
					return ERROR_FLAG;
			
				if(pointer!=NULL)
				{
					strcpy(pointer-> binary_code,"0000000000");/*Initialize the coding with a line of zeros*/
					pointer -> binary_code[10] = '\0';
					
					pointer -> ic  = IC; 

					IC += 1;/*increase the IC*/
					if(index_words == 1)/*If we are in the second operand we will code in bits 7-4*/
					{
						for(j=7;j>4;j--)
						{
							if(i % 2==1)
								pointer-> binary_code[j] = '1';
							i/= 2;
						}
					}
					else/*If we are in the first operand we will code in bits 0-3*/
					{
						for(j=3;j>0;j--)
						{
							if(i % 2==1)
								pointer-> binary_code[j] = '1';
							i /= 2;
						}
					}
				}
			}
			
			else/*If we are in the second operand and there was no previous register, we will code in a new line of coding.*/
			{
				if(pointer != NULL)
				{
					pointer ->binary_code [4] = '0'; 
					for(j=7;j>4;j--)
					{
						if(i % 2==1)
							pointer-> binary_code[j] = '1';
						else
							pointer-> binary_code[j] = '0';
						i /= 2;
					}
				}
			}
			/*In the coding line of the opcode, we will mark that this operand has register addressing.*/
			
			head_op_code -> binary_code[index] = '1';
			head_op_code-> binary_code[index+1] = '1';


	
		}
		else
		{
			if(add_instruction(&pointer) == True)
				return ERROR_FLAG;


			

			if(pointer!=NULL)
			{	
				pointer -> ic  = IC; 
				IC += 1;
				strcpy(pointer-> binary_code,"0000000000");
				pointer -> binary_code[10] = '\0';
				if(words[index_words][0]=='#')/*Checks if the operand is an immediate number*/
				{
					for(i=1;i<strlen(words[index_words]) && isspace(words[index_words][i]); i++);/*Go over the white characters*/
					if(i< strlen(words[index_words]) && (words[index_words][i] == '-' || words[index_words][i] == '+'))/*Checks if attached to a number, plus or minus*/
						i += 1;
					for(j=i; j<strlen(words[index_words]) && isdigit(words[index_words][j]); j++);/*Checks if each character of the number is a digit*/
					for(i=j; i<strlen(words[index_words]) && isspace(words[index_words][i]); i++);/*Go over the white characters*/
					if(i != strlen(words[index_words]))/*If there are more characters that are not adjacent to the number*/
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal number.\n");
						return ERROR_FLAG;
					}
					strcpy(string_num,words[index_words]+1);
					/*Converts the string to a number*/
					num = (int)strtod(string_num,&rest_line);


					if(num<0)
					{
						pointer -> binary_code[0] = '1';
					}
					num = 1023 & num;/*in case the number is negative*/
					if(strcmp(words[index_words]+1,rest_line)==0)/*In case no number is entered after the #*/
					{
						printf("In line: %d\t",line_number);
						printf("Error. Excepted number.\n");
						return ERROR_FLAG;
					}
					else/*The number is coded in binary coding, and there is no need to mark the opcode line, because it is initialized with 00*/
					{
					
						for(j=7;j>=0;j--)
						{
							if(num % 2==1)
								pointer-> binary_code[j] = '1';
							num/= 2;
						}
				 	}
				}
				else/*In case it is a label, we will mark it in the list of undefined labels, and handle it in the second pass*/
				{
					for(j=0;j<strlen(words[index_words]) && words[index_words][j]!='.'; j++);/*Checks if there is a period in the word and the operand is of the record access address type.*/
					strcpy(pointer-> binary_code,"?");
					for(k=j;k>0 && isspace(words[index_words][k]);k--);
					/*The opperand that suppose to be lable (in a direct or immediate address
	), since other options
					were rejected. 
					Check if the lable is legal*/
					for(i=0;i<strlen(words[index_words]) && isspace(words[index_words][i]);i++);
					if(strlen(words[index_words])==0 || !isalpha(words[index_words][i]))
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal opernd.\n");
						return ERROR_FLAG;
					}
					for(i = i+1; i<k && isalnum(words[index_words][i]); i++);
					if(i!=k)
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal opernd.\n");
						return ERROR_FLAG;
					}

					if(pointer!=NULL && undefined_lable_temp != NULL)
					{


						undefined_lable_temp -> ic = pointer->ic;
					
						undefined_lable_temp -> instruction = pointer;
					
						undefined_lable_temp -> line_number = line_number;
						strncpy(undefined_lable_temp -> lable_name,words[index_words],k);
							/*After we inserted the undefined label we will allocate space to the next member in the list of undefined labels.*/
						undefined_lable_temp -> lable_name[k]='\0';

						undefined_lable_temp ->next = (undefined_lable*)malloc(sizeof(undefined_lable));
						if(undefined_lable_temp->next== NULL)
						{
							printf("Error. Not enough memory.\n");
							return ERROR_FLAG;
						}
					
						undefined_lable_temp = undefined_lable_temp->next;
						undefined_lable_temp->instruction = NULL;
						undefined_lable_temp->next = NULL;
					}
					/*If this is a normal label*/
					if(j == strlen(words[index_words]))
					{
						head_op_code->binary_code[index]='0';
						head_op_code->binary_code[index+1]='1';	
					}
				
					else
					{
						head_op_code->binary_code[index]='1';
						head_op_code->binary_code[index+1]='0';
						if( add_instruction(&pointer)==True)
							return ERROR_FLAG;
					/*Checks that I accessed the allowed places in the struct, and encodes the location in a new line of code.*/
						if(pointer != NULL) {	
							strcpy(pointer-> binary_code,"0000000000");
							pointer -> ic  = IC; 
							IC += 1;
							num = (int)strtod(words[index_words]+j+1,&rest_line);
							if(num<1 || num>2)
							{
								printf("In line: %d\t",line_number);
								printf("Error. Illegal opernd.\n");
								return ERROR_FLAG;
							}
							num = 1023 & num;
							if(strcmp(words[index_words]+1,rest_line)==0)
							{
								printf("In line: %d\t",line_number);
								printf("Error. Excepted number.\n");
								return ERROR_FLAG;
							}
						
							for(j=7;j>=0;j--)
							{
								if(num % 2==1)
									pointer-> binary_code[j] = '1';
								num/= 2;
							}
						}
							
					}	
				}
			
			}
		}
		index +=2;/*Move to the 2 bits of the target operand.*/
		index_words += 1;/*Move to the next operand*/

	}
	if(token != NULL)/*If there are more characters after the 2 operands.*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Too many arguments.\n");
		return ERROR_FLAG;
	}
	
	if(line[start] == COMMA || counter > 1)
	{
		printf("In line: %d\t",line_number);
		printf("Error. Extra comma.\n");
		return ERROR_FLAG;	
	}
	return IC;
}

int find_opcode_1_operands(int line_number,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{
/*Similar to the function of checking 2 operands, only in this case there is one operand. The tests are the same idea.*/
	char registers[REGISTERS_LENGTH][3] = {"r0","r1","r2","r3","r4","r5","r6","r7"};
	char extract_word[MAX_LENGTH_OF_LINE];
	char *rest_line = NULL;
	instruction *head_op_code = pointer;


	int i=0,k,j,num,counter = 0;
	for(i=0;i<strlen(line) && isspace(line[i]);i++);
	if(i == strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. Excepted arguments.\n");
		return ERROR_FLAG;
	
	}
	for(k=i; k<strlen(line); k++)
	{
		if(line[k] == COMMA)
			counter += 1;
		else
		{
			if(!isspace(line[k]) && counter > 0)
			{
				printf("In line: %d\t",line_number);
				printf("Error. Excepted only one argument.\n");
				return ERROR_FLAG;
			}
		}
	}
	if(counter > 0)
	{
		printf("In line: %d\t",line_number);
		printf("Error. Extra comma.\n");
		return ERROR_FLAG;	
	}
	for(i=0; i<strlen(line) && isspace(line[i]);i++);
	for(j=i; j<strlen(line) && !isspace(line[j]);j++);
	strncpy(extract_word,line+i,j-i);
	extract_word[j-i] = '\0';
	for(i=j;i<strlen(line) && isspace(line[i]); i++);


	if(i<strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. Illegal opernd.\n");
		return ERROR_FLAG;
	}
	if( add_instruction(&pointer)==True)
		return ERROR_FLAG;
	if(pointer != NULL)
	{
		strcpy(pointer-> binary_code,"0000000000");
		pointer -> binary_code[10] = '\0';
		pointer -> ic  = IC; 
		IC += 1;
	}
	for(i=0;i<REGISTERS_LENGTH && strcmp(extract_word,registers[i])!=0;i++);
	if(i!=REGISTERS_LENGTH)
	{				
		head_op_code -> binary_code[6] = '1';
		head_op_code-> binary_code[7] = '1';
		for(j=7;j>4;j--)
		{
			if(i % 2==1)
				pointer-> binary_code[j] = '1';
			i/= 2;
		}

	}
	else
	{
		if(extract_word[0]=='#')
		{
			for(i=1;i<strlen(extract_word) && isspace(extract_word[i]); i++);
			if(i< strlen(extract_word) && (extract_word[i] == '-' ||extract_word[i] == '+'))
				i += 1;
			for(j=i; j<strlen(extract_word) && isdigit(extract_word[j]); j++);
			for(i=j; i<strlen(extract_word) && isspace(extract_word[i]); i++);
			if(i != strlen(extract_word))
			{
				printf("In line: %d\t",line_number);
				printf("Error. Illegal number.\n");
				return ERROR_FLAG;
			}
			num = (int)strtod(extract_word+1,&rest_line);
			if(num<0)
			{
				pointer -> binary_code[0] = '1';
			}
			num = 1023 & num;
			if(strcmp(extract_word+1,rest_line)==0)
			{
				printf("In line: %d\t",line_number);
				printf("Error. Excepted number.\n");
				return ERROR_FLAG;
			}
			else
			{					
				for(j=7;j>=0;j--)
				{
					if(num % 2==1)
						pointer-> binary_code[j] = '1';
					num/= 2;
				}
		 	}
		}
		else
		{
			for(j=0;j<strlen(extract_word) &&  extract_word[j]!='.'; j++);
			for(k=j;k>0 && isspace(extract_word[k]);k--);
			/*The opperand that suppose to be lable (in a direct or immediate address
), since other options
			were rejected. 
			Check if the lable is legal*/
			for(i=0;i<strlen(extract_word) && isspace(extract_word[i]);i++);
			if(strlen(extract_word)==0 || !isalpha(extract_word[i]))
			{
				printf("In line: %d\t",line_number);
				printf("Error. Illegal opernd.\n");
				return ERROR_FLAG;
			}
			for(i = i+1; i<k && isalnum(extract_word[i]); i++);
			if(i!=k)
			{
				printf("In line: %d\t",line_number);
				printf("Error. Illegal opernd.\n");
				return ERROR_FLAG;
			}
			if(pointer!=NULL)
			{
				undefined_lable_temp -> ic = pointer->ic;
				
				undefined_lable_temp -> instruction = pointer;
				undefined_lable_temp -> line_number = line_number;
				
				strncpy(undefined_lable_temp -> lable_name,extract_word,k);
				undefined_lable_temp -> lable_name[k]='\0';

				
				undefined_lable_temp ->next = (undefined_lable*)malloc(sizeof(undefined_lable));
				if(undefined_lable_temp== NULL)
				{
					printf("In line: %d\t",line_number);
					printf("Error. Not enough memory.\n");
					return ERROR_FLAG;
				}
				undefined_lable_temp = undefined_lable_temp->next;
				undefined_lable_temp->next = NULL;
				undefined_lable_temp->instruction = NULL;
				
			}
			
			 
			strcpy(pointer-> binary_code,"?");
			if(j == strlen(extract_word))
			{
				head_op_code->binary_code[6]='0';
				head_op_code->binary_code[7]='1';	
			}
			else
			{
				head_op_code->binary_code[6]='1';
				head_op_code->binary_code[7]='0';
				if(add_instruction(&pointer) == True)
					return ERROR_FLAG;
				
				if(pointer != NULL)
				{
					strcpy(pointer-> binary_code,"0000000000");
					pointer -> ic  = IC; 
					IC += 1;
					num = (int)strtod(extract_word+j+1,&rest_line);
					if(num<1 || num>2)
						{
							printf("In line: %d\t",line_number);
							printf("Error. Illegal opernd.\n");
							return ERROR_FLAG;
						}
					num = 1023 & num;
					if(strcmp(extract_word+1,rest_line)==0)
					{
						printf("In line: %d\t",line_number);
						printf("Error. Excepted number.\n");
						return ERROR_FLAG;
					}
					for(j=7;j>=0;j--)
					{
						if(num % 2==1)
							pointer-> binary_code[j] = '1';
						num/= 2;
					}
				}
						
			}	
		}
	}
	return IC;
				
}

/*receives 2 operands and the target operand cannot receive an immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_mov(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC; 
	IC = find_opcode_2_operands(line_number,line,pointer,IC+1, undefined_lable_temp);
	if(pointer->binary_code != NULL && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0') && IC != ERROR_FLAG)/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Receives 2 operands and both of them all types of addresses.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_cmp(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC; 
	IC = find_opcode_2_operands(line_number,line,pointer,IC+1,undefined_lable_temp);	
	return IC;
}

/*receives 2 operands and the target operand cannot receive an immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_add(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC; 
	IC = find_opcode_2_operands(line_number,line,pointer,IC+1,undefined_lable_temp);
	if(pointer->binary_code != NULL && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0') && IC != ERROR_FLAG)/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;
}

/*receives 2 operands and the target operand cannot receive an immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_sub(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{
	pointer -> ic  = IC; 
	IC = find_opcode_2_operands(line_number,line,pointer,IC+1,undefined_lable_temp);
	if(pointer->binary_code != NULL && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0') && IC != ERROR_FLAG)/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;
}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_not(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{ 

	pointer -> ic  = IC;
	IC +=1; 
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0') && IC != ERROR_FLAG)/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

	
}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_clr(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1;
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0') && IC != ERROR_FLAG)/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;
}

/*Receives 2 operands, the source operand can only receive a direct address or a record access address. and the target operand cannot receive an immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_lea(int line_number,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC; 
	IC = find_opcode_2_operands(line_number,line,pointer,IC+1,undefined_lable_temp);
	if(IC != ERROR_FLAG && pointer->binary_code != NULL && ((pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0')||(pointer->binary_code[4] == '0' && pointer->binary_code[5] == '0')||(pointer->binary_code[4] == '1' && pointer->binary_code[5] == '1')) )/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_inc(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC = find_opcode_1_operands(line_number,line,pointer,IC+1,undefined_lable_temp);
	if(pointer->binary_code != NULL && IC != ERROR_FLAG &&  (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}
/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_dec(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1;
	IC = find_opcode_1_operands(line_number,line, pointer, IC ,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && IC != ERROR_FLAG &&  (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC; 

}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_jmp(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1;
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && IC != ERROR_FLAG && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_bne(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1; 
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && IC != ERROR_FLAG && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_get(int line_number ,char* line, instruction* pointer, int IC,undefined_lable* undefined_lable_temp)
{

	pointer -> ic  = IC; 
	IC += 1;
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && IC != ERROR_FLAG && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Receives one operand.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_prn(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1; 
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	return IC;

}

/*Receives one operand and it cannot be in immediate address.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_jsr(int line_number ,char* line, instruction* pointer, int IC,	undefined_lable * undefined_lable_temp)
{

	pointer -> ic  = IC;
	IC += 1;
	IC = find_opcode_1_operands(line_number,line, pointer, IC,undefined_lable_temp) ;
	if(pointer->binary_code != NULL && IC != ERROR_FLAG && (pointer->binary_code[6] == '0' && pointer->binary_code[7] == '0'))/*Checks if the opernd is compatible*/
	{
		printf("In line: %d\t",line_number);
		printf("Error. Wrong operand to function.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Does not accept operands, if anything is entered after the opcode an error will be received.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_rts(int line_number ,char* line, instruction* pointer, int IC,undefined_lable * undefined_lable_temp)
{
	int i;
	pointer -> ic  = IC; 
	IC += 1;
	for(i=0; i<strlen(line) && isspace(line[i]);i++);
	if(i!=strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. No operand excepted.\n");
		return ERROR_FLAG;
	}
	return IC;

}

/*Does not accept operands, if anything is entered after the opcode an error will be received.
Returns current IC if successfull. ERROR_FLAG oter*/
int read_hlt(int line_number ,char* line, instruction* pointer, int IC,undefined_lable * undefined_lable_temp)
{
	int i;
	pointer -> ic  = IC; 
	IC += 1;
	for(i=0; i<strlen(line) && isspace(line[i]);i++);
	if(i!=strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. No operand excepted.\n");
		return ERROR_FLAG;
	}
	return IC; 

}
