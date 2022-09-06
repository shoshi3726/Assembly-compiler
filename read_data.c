
#include "transition1.h"

int read_numbers(int line_number ,char* line, data* pointer,int DC)
{
	char *rest_line =NULL;
	char* token = NULL;
	int i = 0, num;
	double extract_num;
	const char s[2] = ",";
	boolean was_num = False;
	for(i=0;i<strlen(line) && isspace(line[i]);i++);
	if(i == strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. Missing numbers.\n");
		return ERROR_FLAG;
	
	}
	/*If there is a comma before, it is an error.*/
	for(i=0;i<strlen(line);i++)
	{
		if(!isspace(line[i]) && line[i] != ',')	
			was_num = True;
		if(line[i] == ',')
		{
			if(was_num == True)
				was_num = False;
			else 
			{
				printf("In line: %d\t",line_number);

				printf("Error. Extraneous comma.\n");
				return ERROR_FLAG;
			}
		}
	}
	/*If there is a comma after, it is an error.*/
	for(i=(strlen(line)-1);i>0&&isspace(line[i]);i--);
	if(line[i]==',')
	{
		printf("In line: %d\t",line_number);
		printf("Error. Extraneous comma.\n");
		return ERROR_FLAG;
	}
	/*Read numbers and check their correctness. And in the binary code, put the number converted to binary*/
	token = strtok(line, s);
   
	while( token != NULL && line!=NULL ) {

		extract_num = strtod(token,&rest_line);
		num = (int)extract_num;
		/*Checks if the number is not an integer*/
		if(num < extract_num)
		{
			printf("In line: %d\t",line_number);
			printf("Error. Excepted only integers.\n");
			return ERROR_FLAG;
		}

		if(num<0)
		{
			pointer -> binary_code[0] = '1';
		}
		else
			pointer -> binary_code[0] = '0';
		num = 1023 & num;
		if(strcmp(token,rest_line)==0)
		{
			printf("In line: %d\t",line_number);
			printf("Error. Excepted only numbers.\n");
			return ERROR_FLAG;
		}


		token = strtok(NULL, s);
		if(pointer != NULL)
		{
			pointer -> binary_code[10] = '\0';
			for(i=9;i>0;i--)
			{
				if(num % 2==1)
					pointer-> binary_code[i] = '1';
				else
					pointer-> binary_code[i] = '0';
				num /= 2;
			}

			pointer -> dc  = DC; 
		}
		DC += 1;

	}
	if(token!= NULL)
	{
		/*Updating the binary code of the data*/
		num = (int)strtod(token,&rest_line);

		if(num<0)
		{
			pointer -> binary_code[0] = '1';
		}
		else
			pointer -> binary_code[0] = '0';
			

		num = 1023 & num;
		if(strcmp(token,rest_line)==0)
		{
			printf("In line: %d\t",line_number);
			printf("Error. Excepted only numbers.\n");
			return ERROR_FLAG;
		}
		
		pointer -> binary_code[10] = '\0';
		for(i=9;i>0;i--)
		{
			if(num % 2==1)
				pointer-> binary_code[i] = '1';
			else
				pointer-> binary_code[i] = '0';
			num /= 2;
		}

		pointer -> dc  = DC;
		/*Assign a new member to the data list and advance the DC*/
		if(add_data(&pointer) == True)
			return ERROR_FLAG;

		DC += 1;
}

	return DC;
	
}

int read_string(int line_number ,char* line, data* pointer,int DC)
{
	int i,j,start,end = strlen(line);	

	int ascii;
	for(start = 0;start<end && isspace(line[start]); start++);
	if( start == end)
	{
		printf("In line: %d\t",line_number);
		printf("Error. Excepted string.\n");
		return ERROR_FLAG;
	}
	for(end = strlen(line)-1;end>start && isspace(line[end]); end--);
	if( start == end || line[start]!='"' || line[end] != '"')
	{
		printf("In line: %d\t",line_number);
		printf("Error. Illegal string.\n");
		return ERROR_FLAG;
	}
	/*Divides each word into characters and assigns each a coding line*/
	start += 1;
	for(i = start; i < end; i++)
	{
		
		ascii = line[i];
		pointer-> binary_code[0]='0';
		pointer-> binary_code[10]='\0';
		for(j=9;j>0;j--)
		{
			if(ascii % 2==1)
				pointer-> binary_code[j] = '1';
			else
				pointer-> binary_code[j] = '0';
			ascii /= 2;
		}
		pointer -> dc = DC;
		if(add_data(&pointer) == True)
			return ERROR_FLAG;
		DC+=1;

	}
	/*Add another data line that represents \0, meaning the end of the word, and in its binary coding, there are only zeros.*/
	strcpy(pointer-> binary_code,"0000000000");
	

	pointer -> dc = DC;
	if(add_data(&pointer) == True)
			return ERROR_FLAG;
	DC+=1;
	
	
	return DC;
}

int read_struct(int line_number ,char* line, data* pointer,int DC)
{
	char* token = NULL;
	char original_line [80];
	char temp[81];
	boolean error = False;
	int i = 0, len = 0;
	const char s[2] = ",";
	for(i=0;i<strlen(line) && isspace(line[i]);i++);
	if(i == strlen(line))
	{
		printf("In line: %d\t",line_number);
		printf("Error. Excepted arguments.\n");
		return ERROR_FLAG;
	
	}
	strcpy(original_line,line);
	token = strtok(line, s);
   	/*The first word in the struct we will read using read_numbers and the second word using read_string*/
	if( token != NULL && line!=NULL ) {
		strcpy(temp,token);
		DC = read_numbers(line_number,temp, pointer, DC);
		while((pointer -> next) != NULL)
			pointer = pointer->next;
		len += (strlen(token)+1);
		strcpy(temp,token);
		DC = read_string(line_number,original_line+len, pointer, DC);
		if(DC == ERROR_FLAG)
			error = True;

	}
	if(error == True)
		return ERROR_FLAG;
	return DC;
}




