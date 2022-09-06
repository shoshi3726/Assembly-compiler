

#include "transition1.h"



int second_pass(char* fullname,instruction * head_instruction, data* head_data,lable* head_lable,undefined_lable* undefined_lable_head)
{

	int i,start=0,j,num=0,line_number = 1;
	char line[MAX_LENGTH_OF_LINE],full_line[MAX_LENGTH_OF_LINE],original_line [MAX_LENGTH_OF_LINE],extract_word[MAX_LENGTH_OF_LINE],name_of_lable[MAX_LENGTH_OF_LINE];
	FILE * file;
	char* token;
	const char seperate_words[4] = ":";
	boolean error = False,was_lable = False;

	undefined_lable* undefined_lable_temp = undefined_lable_head;

	lable* temp_lable = head_lable;
	lable *temp_lable2=head_lable;
	file = fopen(fullname, "r");

	if(file==NULL)
	{
		printf("Error. Can't open file. exit.\n");
		return ERROR_FLAG;
	}
	/*Encoding all the lables which were used in the instruction and didnt get a code in the first pass*/
	while(undefined_lable_temp!=NULL && undefined_lable_temp->next!=NULL)
	{
		/*Checks if the lable wich used was defined*/
		for(temp_lable = head_lable;temp_lable!=NULL && temp_lable->next!= NULL && strcmp(temp_lable->name,undefined_lable_temp->lable_name)!=0;temp_lable = temp_lable->next);

		
		if(temp_lable ==  NULL || temp_lable->next == NULL )
		{
			printf("In line: %d\t",undefined_lable_temp -> line_number);
			printf("Error. Using lable %s without definition.\n",undefined_lable_temp -> lable_name); 
			error = True;
		}
		else
		{ 
			if(undefined_lable_temp -> instruction!=NULL)
			{
				/*Encode the lable in the IC which it was used*/
				strcpy((undefined_lable_temp -> instruction)->binary_code,"0000000000");

				if(strcmp(temp_lable -> sign,"external")==0)
				{
					(undefined_lable_temp -> instruction)->binary_code[9]='1';
				}
				else
				{
					(undefined_lable_temp -> instruction)->binary_code[8]='1';
					num = temp_lable->adress;

					for(i=7;i>=0;i--)
					{
						if(num%2==1)
						{
							(undefined_lable_temp -> instruction)->binary_code[i] = '1';	
						}
						num /= 2;
					}
				}
				
			}
		}
		undefined_lable_temp = undefined_lable_temp -> next;
	}	
	/*Read the file again in order to check the entries*/
	while(fgets(line,MAX_LENGTH_OF_LINE,file))
	{
		/*Checks if a lable was defined*/
		strcpy(full_line,line);
		strcpy(original_line,line);
		token = strtok(line, seperate_words);
		if(strcmp(token,original_line)!=0)
		{
			for(i=0;i<strlen(token) && isspace(token[i]);i++);
			strcpy(name_of_lable,token+i);
			for(i=0;i<strlen(name_of_lable) && !isspace(name_of_lable[i]); i++);	
			name_of_lable[i] = '\0';
			token = strtok(NULL, seperate_words);
			strcpy(original_line,token);
			was_lable = True;

		}
		/*Extract the word after the lable*/
		for(start=0; original_line != NULL && start<strlen(original_line) && isspace(original_line[start]);start++);
		for(i=0; original_line != NULL && (i+start)<strlen(original_line) && !isspace(original_line[i+start]);i++)
		{
			extract_word[i] = original_line[i+start];
		}
		start += i;
		extract_word[i] = '\0';
		if(!(strcmp(".extern",extract_word)==0 || strcmp(".struct",extract_word)==0 || strcmp(".string",extract_word)==0 || strcmp(".data",extract_word)==0))
		{
			if(strcmp(".entry",extract_word)==0)
			{
				
				for(i=start; original_line != NULL && i<strlen(original_line) && isspace(original_line[i]);i++);
				
				for(j=0; original_line != NULL && (j+i)<strlen(original_line) && !isspace(original_line[j+i]);j++)
				{
					extract_word[j] = original_line[j+i];
				}
				extract_word[j] = '\0';
				/*Checks the rest of the line after name of lable*/
				for(i=(j+i);i<strlen(original_line) && isspace(original_line[i]);i++);
				if(i!=strlen(original_line)){
						printf("In line: %d\t",line_number);
						printf("Error. There is more arguments after the lable name.\n");
						error = True;
					}	
				if(was_lable == True)
				{
				
					temp_lable2 = head_lable;
					for(temp_lable = head_lable;temp_lable!=NULL && temp_lable->next!=NULL && strcmp((temp_lable->next)->name,name_of_lable)!=0;temp_lable = temp_lable->next);
					if(temp_lable!=NULL && temp_lable->next!=NULL)
					{
						temp_lable2 = temp_lable->next;
						temp_lable->next = (temp_lable->next)->next;
						free(temp_lable2);
					}
					else
					{
						if(temp_lable2!=NULL && strcmp(temp_lable2->name,name_of_lable) == 0)
						{
							temp_lable2 = temp_lable2->next;
							free(head_lable);
							head_lable = temp_lable2;
						}
					}
					printf("In line: %d\t Warning. %s: Meaningless label.\n",line_number,name_of_lable);
						
				}
				/*Checks the entry lable had been defined*/
				for(temp_lable = head_lable;temp_lable!=NULL && strcmp(temp_lable->name,extract_word)!=0;temp_lable = temp_lable->next);
				if(temp_lable == NULL || temp_lable->next == NULL) 
				{
					printf("In line: %d\t",line_number);
					printf("Error. Undefined entry lable. \n");
					error = True;
				}
				else
				{
					/*Checks if the lable defined already as extern*/
					if(strcmp(temp_lable->sign,"extern")==0)
					{
						printf("In line: %d\t",line_number);
						printf("Error. A label cannot be defined as both entery and extern.\n");
						error = True;
					}
					else
					{
						strcpy(temp_lable->sign,"entry");
					}
				}	
			}
		
		}
		was_lable = False;
		line_number += 1;
	}

	if(error == True)
		return ERROR_FLAG;
	return SUCCESS_FLAG;

}

 
