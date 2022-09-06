

#include "macro.h"

typedef enum {
   False, True
} boolean;

int open_macro(char * name)
{
	/*Initialize the line number, by checking if we didn't use Macao before its declaration.
		and result in order to receive the value returned from the creation of the file by the "create_file_am" function, and check whether an ERROR FLAG or a SUCCESS FLAG was received.*/
	int i=0,line_counter = 0,result = 0;
	/*temporary pointer to char, in order to keep the original line.*/
	char original_line[MAX_LENGTH_OF_LINE1];
	boolean is_macro = False;
	char * fullname = (char*) malloc((strlen(name)+4)*sizeof(char));
	char line[MAX_LENGTH_OF_LINE1];
	/*The separator words for the strtok function*/
	const char seperate_words[4] = " \t\n";
	char *token = NULL;
	char first_word[MAX_LENGTH_OF_LINE1];
	char name_of_macro[MAX_LENGTH_OF_LINE1];
	/*Initializing the first member in the array of macros*/
	macro* macros = (macro*)malloc(sizeof(macro));
	/*A temporary variable for reallocing the macro array*/
	macro* tmp_macro = macros;
	/*Saves the number of macros*/
	int macros_length = 0;
	FILE * fp = NULL;
	/*Check if there is an error in each line and note it, but still continue to check the rest of the file.
	If there is an error, do not continue to the first pass.*/ 
	boolean error = False,error_line = False;
	/*Checking if we were able to allocate memory.*/
	if(fullname == NULL )
	{
		if(macros != NULL) free(macros);
		printf("Error. There is not enough memory.\n");
		return ERROR_FLAG;
	}
	if(macros == NULL)
	{
		printf("Error. There is not enough memory.\n");
		error = True;
	}
	/*Add .as to the name of the entered file, and open it.*/
	strcpy(fullname,name);
	strcat(fullname,".as");


	fp = fopen(fullname,"r");
	if(fp == NULL)
	{
		printf("Error. there is no such file.\n");
		return ERROR_FLAG;
	}
	/*Start reading line by line and check using strtok the first and second word (if necessary)*/
	while(fgets(line,MAX_LENGTH_OF_LINE1,fp))
	{	
			line_counter += 1;/*Increase the line counter*/	
			strcpy(original_line,line);/*Keep the line at temp*/

			token = strtok(line, seperate_words);/*Separate using the separating words*/

			if(token != NULL ) /*If we managed to separate, we will keep the first word*/ 
			{
				strcpy(first_word,token);
				token = strtok(NULL, seperate_words);
			}
			/*If we are inside a macro, we will check if we have reached the end of the macro, and we will allocate space for the next macro. If we haven't reached the end, we will add the line to the macro content.*/
			if(is_macro == True)
			{
				if(strcmp(first_word,ENDMACRO)==0)
				{
					is_macro = False;
					macros_length+=1;
					if(tmp_macro != NULL) {
						tmp_macro = (macro*)realloc(macros,(sizeof(macro))*(macros_length+1));
						/*Check if it was successful*/
						if(tmp_macro == NULL)
						{
							printf("In line: %d\tError. There is not enough memory.\n",line_counter);
							error_line = True;
						}
					macros = tmp_macro;
					}
			
				}
				else
				{
					if(macros != NULL)
						strcat((*(macros+macros_length)).content,original_line);
				}
			}
			else
			{
				if(strcmp(first_word,MACRO)==0)
				{
					is_macro = True;
					/*If the line after the macro name is not empty, then the macro name is incorrect.*/
					if( token != NULL ) 
					{	
						strcpy(name_of_macro,token);
						token = strtok(NULL, seperate_words);
						if(token != NULL && error_line == False)
						{
							printf("In line: %d\tError. Theres too many arguments after the definition of macro\n",line_counter);
							error_line = True;
						}	
					}
					/*If there is nothing after the word macro*/
					else
					{
							printf("In line: %d\tError. Missing name of macro\n",line_counter);
							error_line = True;
					}
					/*If this name has already been used for another macro.*/
					for(i=0; i<macros_length && macros != NULL; i++)
					{
						if(strcmp(name_of_macro,(*(macros+i)).name)==0)
						{
							printf("In line: %d\tError. Duplicate declaration of macro %s\n",line_counter,name_of_macro);	
							error_line = True;
						}
					}
					/*If everything is correct, we will enter the name and the line number of the array of macros that was already allocated earlier.*/
					if(macros != NULL)
					{
						strcpy((*(macros+macros_length)).name,name_of_macro);
						(*(macros+macros_length)).line_of_macro = line_counter;
					}
			
				}
			} 
		/*If there was an error in the line we will mark that there is an error in the file*/
			if(error_line == True)
			{
				error = True;
				}
				/*In order to move to the next line, mark that there is no error in the line.*/
			error_line = False;
	}
	if(error == True)
		return ERROR_FLAG;
	/*If there were no errors, we will move on to creating the file*/
	result = create_file_am(name,macros,macros_length);	
	free(fullname);
	/*If there were macros we will release the array*/
	if(macros != NULL)
		free(macros);
	fclose(fp); 
	return result;
}

int create_file_am(char* name,macro* macros,int macros_length)
{
	char line[MAX_LENGTH_OF_LINE1];
	const char seperate_words[4] = " \t\n";
	char original_line[MAX_LENGTH_OF_LINE1];
	boolean is_macro = False,error = False;
	char first_word[MAX_LENGTH_OF_LINE1];
	int i,line_counter = 0;
	char* token;
	/*Allocate space for the file names plus the .as or .am suffix and open the files*/
	char * fullname_am = NULL;
	char * fullname_as = NULL;
	FILE * file_am=NULL;
	FILE * file_as=NULL;
	fullname_as=(char*)malloc((sizeof(char))*(strlen(name)+5));
	fullname_am=(char*)malloc((sizeof(char))*(strlen(name)+5));
	if(fullname_am==NULL || fullname_as==NULL)
	{
		if(fullname_am!= NULL) free(fullname_am);
		if(fullname_as!= NULL) free(fullname_as);
		printf("Error. There is not enough memory. Can't create file\n");
		return ERROR_FLAG;
	}
	strcpy(fullname_as,name);
	strcpy(fullname_am,name);
	strcat(fullname_am,".am");
	strcat(fullname_as,".as");
	file_am = fopen(fullname_am, "w+");
	file_as = fopen(fullname_as, "r");
	if(file_am==NULL || file_as==NULL)
	{
		if(file_am!=NULL) fclose(file_am);
		if(file_as!=NULL) fclose(file_as);
		printf("Error.  Can't create file\n");
		return ERROR_FLAG;
	}
	/*Read line by line and if you come across a macro name, copy the macro content instead.
	And if you encounter a macro definition, do not copy to the .am file until you encounter endmacro*/
	while(fgets(line,MAX_LENGTH_OF_LINE1,file_as))
	{
		line_counter += 1;
		strcpy(original_line,line);
		token = strtok(line, seperate_words);
		/*With strtok you check the first word in each line.
		If it's a macro, an endmacro, or a single word on a line that means using a macro.*/
		if(token != NULL ) 
		{
			strcpy(first_word,token);
			token = strtok(NULL, seperate_words);
		}
		if(is_macro == True)
		{
			if(strcmp(first_word,ENDMACRO)==0)
			{
				is_macro = False;
			}
		}
		else
		{
			if(strcmp(first_word,MACRO)==0)
				is_macro = True;
			else
			{
				for(i=0; i<macros_length; i++)
				{
					if(strcmp(first_word,(*(macros+i)).name)==0)
						break;	
				}	
				if(i<macros_length)
				{
					if((*(macros+i)).line_of_macro > line_counter)
					{
						printf("In line: %d\tUsing macro before definition.\n",line_counter);
						error = True;
					}
					fputs((*(macros+i)).content, file_am);
				}
				else
					fputs(original_line, file_am);	
	
			}
	

		}
		first_word[0]='\0';	
	}
	free(fullname_am);
	free(fullname_as);
	fclose(file_am);
	fclose(file_as);
	if(error == True)
		return ERROR_FLAG;
	return SUCCESS_FLAG;	

}
