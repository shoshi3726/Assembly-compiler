

#include "transition1.h"



int add_lable(lable ** temp_lables)
{
		if(temp_lables == NULL || (*temp_lables) ==NULL)
			return True;
		(*temp_lables) -> next = (lable*)malloc(sizeof(lable));

		if((*temp_lables) -> next != NULL)
		{
			(*temp_lables) = (*temp_lables) -> next;
			(*temp_lables) -> next = NULL;
			return False;
		}
		else
		{
			printf("Error. Not enogh memory.\n");
			return True;
		}
}


int add_data(data ** temp_data)
{
		if(temp_data == NULL || (*temp_data) ==NULL)
			return True;
		(*temp_data) -> next = (data*)malloc(sizeof(data));

		if((*temp_data) -> next != NULL)
		{
			(*temp_data) = (*temp_data) -> next;
			(*temp_data) -> next = NULL;
			return False;
		}
		else
		{
			printf("Error. Not enogh memory.\n");
			return True;
		}
}

int add_instruction(instruction ** temp_instructions)
{
		if(temp_instructions == NULL || (*temp_instructions) ==NULL)
			return True;

		(*temp_instructions) -> next = (instruction*)malloc(sizeof(instruction));

		if((*temp_instructions) -> next != NULL)
		{
			(*temp_instructions) = (*temp_instructions) -> next;
			(*temp_instructions) -> next = NULL;
			return False;
		}
		else
		{
			printf("Error. Not enogh memory.\n");
			return True;
		}
}


int first_transition(char* name)
{
	undefined_lable* undefined_lable_head=NULL;
	undefined_lable* undefined_lable_temp=NULL;
	char * fullname_am = NULL;
	char line [MAX_LENGTH_OF_LINE],full_line[MAX_LENGTH_OF_LINE];
	int IC = 0;
	int DC = 0;
	int line_number = 1;
	char name_of_lable[31],tmp[MAX_LENGTH_OF_LINE];
	char* token;
	const char seperate_words[4] = ":";
	char original_line [MAX_LENGTH_OF_LINE],extract_word[MAX_LENGTH_OF_LINE];
	int i,j,k,index,num = 0;
	lable* head_lables = (lable*)malloc(sizeof(lable));
	data* head_data = (data*)malloc(sizeof(data));
	instruction* head_instructions = (instruction*)malloc(sizeof(instruction));
	boolean define_lable = False,error_func = False;
	int lables_length = 0;
	op_code op_code_array[16];

	FILE * file_am;
	data* temp_data = head_data;
	instruction* temp_instructions = head_instructions;
	boolean error = False,error_line = False;
	lable* temp_lables = head_lables;
	head_lables -> next = NULL;
	
	fullname_am=(char*)malloc((sizeof(char))*(strlen(name)+5));
	undefined_lable_head = (undefined_lable*)malloc(sizeof(undefined_lable));
	/*Array of function of opcodes*/
	op_code_array[0].name="mov"; op_code_array[0].func=read_mov;
	op_code_array[1].name="cmp"; op_code_array[1].func=read_cmp;
	op_code_array[2].name="add"; op_code_array[2].func=read_add;
	op_code_array[3].name="sub"; op_code_array[3].func=read_sub;
	op_code_array[4].name="not"; op_code_array[4].func=read_not;
	op_code_array[5].name="clr"; op_code_array[5].func=read_clr;
	op_code_array[6].name="lea"; op_code_array[6].func=read_lea;
	op_code_array[7].name="inc"; op_code_array[7].func=read_inc;
	op_code_array[8].name="dec"; op_code_array[8].func=read_dec;
	op_code_array[9].name="jmp"; op_code_array[9].func=read_jmp;
	op_code_array[10].name="bne"; op_code_array[10].func=read_bne;
	op_code_array[11].name="get"; op_code_array[11].func=read_get;
	op_code_array[12].name="prn"; op_code_array[12].func=read_prn;
	op_code_array[13].name="jsr"; op_code_array[13].func=read_jsr;
	op_code_array[14].name="rts";
	op_code_array[14].func=read_rts;
	op_code_array[15].name="hlt"; 
	op_code_array[15].func=read_hlt;
	
	if(fullname_am==NULL|| undefined_lable_head == NULL)
	{
		if(fullname_am != NULL) free(fullname_am);
		if(undefined_lable_head != NULL) free(undefined_lable_head);
		printf("Error. Can't open file. exit.\n");
		return ERROR_FLAG;
	}
	undefined_lable_head ->next = NULL;
	undefined_lable_head ->instruction = NULL;
	undefined_lable_temp = undefined_lable_head;	
	strcpy(fullname_am,name);
	strcat(fullname_am,".am");
	file_am = fopen(fullname_am, "r");
	
	if(file_am==NULL)
	{
		printf("Error. Can't open file. exit.\n");
		return ERROR_FLAG;
	}
	
	if(head_lables == NULL ||head_data == NULL || head_instructions == NULL  )
	{
		printf("Error. There is not enough memory.\n");
		error = True;
	}
	if(error == False)
	{
		head_lables->next = NULL;
		head_data->next = NULL;
		head_instructions -> next = NULL;
	}
	/*Reads all the lines in the file*/
	while(fgets(line,MAX_LENGTH_OF_LINE,file_am))
	{
	
		strcpy(full_line,line);
		/*Clean white spaces*/
		for(i=0;i<strlen(line) && isspace(line[i]); i++);
		if( i != strlen(line) && line[i] != COMMENT && !(strlen(line) > VALID_MAX_LENGTH_OF_LINE ) )
		{
			strcpy(original_line,line);

			token = strtok(line, seperate_words);
			
			if(token != NULL ) 
			{
				if(strcmp(token,original_line)!=0)/*Found a lable*/
				{
					define_lable = True;
					/*Checks the lable name is legal*/
					for(i=0;i<strlen(token)&&isspace(token[i]);i++);
					if(!isalpha(token[i]))
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal labal name.\n");
						error_line = True;
					}
					for(j=i;i<strlen(token) && isalnum(token[j]) ;j++);
					if(j<strlen(token) && error_line==False)
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal labal name.\n");
						error_line = True;
					}
					/*Checks if the name is longer than valid for lable name*/
					if((strlen(token)-i)>30 && error_line==False)
					{
						printf("In line: %d\t",line_number);
						printf("Error. Illegal labal name.\n");
						error_line = True;
					}
					strcpy(name_of_lable,token+i);
			
					token = strtok(NULL, seperate_words);
					/*Checks there is space after lable's decleration*/
					if(!isspace(token[0]) && error_line == False)
					{
						printf("In line: %d\t",line_number);
						printf("Error. Excepted space after lable's decleration.\n");
						error_line = True;
					}
					original_line[0]='\0';
					strcpy(original_line,token);
				}
				strcpy(line,original_line);
				/*Extract the first word of data or instruction*/
				for(i=0; original_line != NULL && i<strlen(original_line) && isspace(original_line[i]);i++);
				strcpy(tmp, original_line+i);
				original_line[0]='\0';
				strcpy(original_line,tmp);

				for(i=0; original_line != NULL && i<strlen(original_line) && !isspace(original_line[i]);i++)
				{
					extract_word[i] = original_line[i];
				}
				extract_word[i] = '\0';
				if(strcmp(".struct",extract_word)==0 || strcmp(".string",extract_word)==0 || strcmp(".data",extract_word)==0) /*There is a Data guidance*/
				{
					if(define_lable == True && error == False) /*If declared a lable, Inserting lable into lables with sign data*/
					{
						lables_length += 1;
					
						strcpy(temp_lables -> name, name_of_lable);
						temp_lables -> adress  = DC + 100; 
						strcpy(temp_lables -> sign,"data");
						if(error_line == False)
							error_line = add_lable(&temp_lables);
						else
							add_lable(&temp_lables);
						
					}
					if(strcmp(".data",extract_word)==0 && error_line == False)	
					{
						DC = read_numbers(line_number,original_line+i,temp_data,DC);/*Encode this line and the numbers wich comes after .data*/
						if(DC == ERROR_FLAG)
						{
							error_line = True;
						}
						while(temp_data != NULL && (temp_data -> next) != NULL ) /*Procceeding the pointer*/
							temp_data = temp_data->next;
					}
					if(strcmp(".string",extract_word)==0 && error_line == False) 
					{
						DC = read_string(line_number,original_line+i,temp_data,DC);/*Encode this line and the string*/	
						if(DC == ERROR_FLAG)
						{
							error_line = True;
						}
						while(temp_data != NULL && (temp_data -> next) != NULL) /*Procceeding the pointer*/
							temp_data = temp_data->next;
					}
					if(strcmp(".struct",extract_word)==0)	
					{
						DC = read_struct(line_number,original_line+i,temp_data,DC); /*Encode this line and the struct*/
						if(DC == ERROR_FLAG)
						{
							error_line = True;
						}
						while(temp_data != NULL && (temp_data -> next) != NULL) /*Procceeding the pointer*/
							temp_data = temp_data->next;
					}		
	

				}
				else 
				{
					if(strcmp(".extern",extract_word)==0 || strcmp(".entry",extract_word)==0)
					{
						if(strcmp(".entry",extract_word)==0)
						{
							/*Checks that there is lable after entry definition*/
							for(k=i;k<strlen(original_line) && isspace(original_line[k]);k++);
							if(k == strlen(original_line))
							{
								printf("In line: %d\t",line_number);
								printf("Error. Missing lable after entry.\n");
								error_line=True;
							}
						}
						if(strcmp(".extern",extract_word)==0)
						{
							for(k=i;k<strlen(original_line) && isspace(original_line[k]);k++);
							/*Checks that there is lable after extern definition*/
							if(k == strlen(original_line))
							{
								printf("In line: %d\t",line_number);
								printf("Error. Missing lable after extern.\n");
								error_line=True;
							}
							/*xtract name of lable*/
							for(j=k;j<strlen(original_line) && !isspace(original_line[j]);j++);

							strncpy(name_of_lable, original_line+k, (j-k));
								name_of_lable[j-k] = '\0';
								/*Chec if the lable already declared in the file*/
								for(temp_lables=head_lables;temp_lables!= NULL && temp_lables->next!=NULL;temp_lables=temp_lables->next)
								{
									if(error_line == False && strcmp(temp_lables->name, name_of_lable)==0){
										printf("In line: %d\t",line_number);
										printf("Error. Already declared in this file.\n");
										error_line=True;
									}
								}
							/*Check if the lable is valid \ if there are more arguments than excepted*/
							if(temp_lables != NULL && error_line == False) {
								strcpy(temp_lables -> name, name_of_lable);
								for(k=j;k<strlen(original_line) && isspace(original_line[k]);k++);
								if(k!=strlen(original_line)){
									printf("In line: %d\t",line_number);
									printf("Error. There is more arguments after the lable name.\n");
									error_line = True;
							}	
							for(i=1;i<strlen(name_of_lable) && isalnum(name_of_lable[i]);i++);
							if(strlen(name_of_lable)==0 || !isalpha(name_of_lable[0]) || i<strlen(name_of_lable))
							{
								printf("In line: %d\t",line_number);
								printf("Error. Illegal lable name.\n");
								error_line = True;
							}

							/*Add the lable to the list with a sign external*/
							if(temp_lables != NULL) {
								strcpy(temp_lables -> sign, "external");
								strcpy(temp_lables -> name, name_of_lable);
								lables_length += 1;
								temp_lables -> adress  = 0;
							}
							if(error_line == False)
								error_line = add_lable(&temp_lables);
							else
								add_lable(&temp_lables);
							}
						}			
					}
					else 
					{
						if(define_lable == True)
						{
							for(temp_lables=head_lables;temp_lables!= NULL && temp_lables->next!=NULL;temp_lables=temp_lables->next)
							{

								if(strcmp(temp_lables->name, name_of_lable)==0)
								{
									printf("In line: %d\t",line_number);
									printf("Error. Already declared in this file.\n");
									error_line=True;
								}
							}
							/*Add the lable with sign code. since it is not data or extern*/
							if(temp_lables!=NULL) {
								lables_length += 1;
								strcpy(temp_lables -> name, name_of_lable);
								temp_lables -> adress  = IC + 100; 
								strcpy(temp_lables -> sign,"code");
							}
							if(error_line == False)
								error_line = add_lable(&temp_lables);
							else
								add_lable(&temp_lables);
						}
						/*Which opcode got in the command */
						for(index=0; index<OP_CODE_LENGTH && strcmp(op_code_array[index].name, extract_word)!=0; index++);

						if(index == OP_CODE_LENGTH && error_line == False)/*Didnt find opcode*/
						{
							printf("In line: %d\t",line_number);
							if(strcmp(extract_word,"\n")==0 && define_lable == True)
							{
								printf("Error. Empty lable\n");
							}
							else
							{
								printf("Error. There is no such command\n");
							}
							error_func = True;
						}
						if(error_func == False)
						{
							/*Encode the line of command and send to the function of specific opcode to encode the addresses*/
							strcpy(temp_instructions->binary_code,"0000000000");
							num = index;

							for(j=3;j>=0;j--)
							{
								if((num % 2)==1)
									temp_instructions-> binary_code[j] = '1';
								num /= 2;
							}
							if(error_line == False)
								IC = (*(op_code_array[index].func))(line_number,original_line+i,temp_instructions,IC,undefined_lable_temp);
							while(undefined_lable_temp!=NULL && undefined_lable_temp->next != NULL) 
								undefined_lable_temp =  undefined_lable_temp->next; 
							while(temp_instructions!=NULL && temp_instructions->next != NULL) 
								temp_instructions =  temp_instructions->next; 
							if(error == False)
								error = add_instruction(&temp_instructions);
							else
								add_instruction(&temp_instructions);
							if(temp_instructions != NULL)
							{
								strcpy(temp_instructions-> binary_code,"0000000000");
								temp_instructions -> binary_code[10] = '\0';
							}
						}
					}
				}
				original_line[0]='\0';
				define_lable = False;
				error_func = False;
				if(error_line == True)
					error = True;
				error_line = False;
			}
		}
		if(strlen(line) > VALID_MAX_LENGTH_OF_LINE ) /*Checks if the line is longer than max valid length of line*/
		{
			printf("In line: %d\t",line_number);
			printf("Error. Line is longer then the max length of line.\n");
			while(line[strlen(line)-1]!='\n')
			{
				fgets(line,MAX_LENGTH_OF_LINE,file_am);
			}
			error = True;
		}
		line_number += 1;

	}
	/*Add IC to al the data*/
	for(temp_lables=head_lables;temp_lables!= NULL && temp_lables->next != NULL;temp_lables=temp_lables->next)
	{
		if(strcmp(temp_lables -> sign, "data")==0)
		{
			temp_lables -> adress  += IC; 
		}
	}
							

	if(error == False)
	{
		/*There were no errors. move to the second pass*/
		if(second_pass(fullname_am,head_instructions,head_data,head_lables,undefined_lable_head) == ERROR_FLAG)
			error = True;

	}
	/*Increasing in 100 all the instructions and data*/
	temp_instructions = head_instructions;
	while(temp_instructions!=NULL && temp_instructions->next!=NULL)
	{
		temp_instructions->ic+=100;
		temp_instructions =  temp_instructions->next;
	}
	temp_data = head_data; 
	while(temp_data!=NULL && temp_data->next!=NULL)
	{
		temp_data->dc+=(100+IC);

		temp_data =  temp_data->next;
	}
	/*If there were no errors found. Create the files*/
	if(error == False)
		error = create_files(name,IC,DC,head_instructions ,head_data,head_lables,undefined_lable_head);
	/*free all the memory which allocated*/
	free(fullname_am);
	free_all_instructions(head_instructions); 
	free_all_data(head_data);
	free_all_lables(head_lables);
	free_all_undefined_lables(undefined_lable_head);
	if(error == True)
		return ERROR_FLAG;
	

	return SUCCESS_FLAG;
}

