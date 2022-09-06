
#include "transition1.h"

char convert_to_32base(char * code)
{
	char base_32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	int index = 0,i;
	/*Algorithm for converting a string of 5 characters representing a binary number, to a base 32 number.*/
	for(i=0;i<5;i++)
	{
		index *= 2;
		if(code[i]=='1')
			index += 1;
	}
	/*Returns a character wich represent the number in 32 base*/
	return base_32[index];

}
int create_files(char * name,int IC, int DC, instruction * head_instruction , data * head_data, lable * head_lable, undefined_lable* head_undefined_lable)
{
	char base_32[32] = {'!','@','#','$','%','^','&','*','<','>','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'};
	/*Counters are initialized in order to check the number of data entered into the file, and if the file is empty it should be deleted.*/
	int index = 0,i,externals_counter = 0,entries_counter = 0;
	/*Name of files to create*/	
	char * fullname_ob = NULL;
	char * fullname_ext = NULL;
	char * fullname_ent = NULL;
	/*Save each half word of the binary encoding*/
	char code1[6];
	char code2[6];
	char code [4];
	/*A temporary variable so that we can calculate the base 32 representation of the IC*/
	int adress = 0;
	/*Since the memory space is limited from address 100 to 256, the size of the address that will be received is 2 characters in base 32.
	Ans also there's place for \n and \0 in the third and fourth places*/
	char ic_code[4];
	/*Initialization of pointers to files that will be opened*/
	FILE * file_ob=NULL;
	FILE * file_ext=NULL;
	FILE * file_ent=NULL;
	/*Initialize pointers to the heads of the lists so that we can go through them.*/
	instruction * temp_instruction =  head_instruction;
	data* temp_data = head_data;
	lable* temp_lable = head_lable;
	undefined_lable* temp_undefined_lable = head_undefined_lable;
	/*Dynamic allocations to the names of the files*/
	fullname_ob=(char*)malloc((sizeof(char))*(strlen(name)+4));
	fullname_ext=(char*)malloc((sizeof(char))*(strlen(name)+5));
	fullname_ent=(char*)malloc((sizeof(char))*(strlen(name)+5));
	/*Check if it was successful*/
	if(fullname_ob==NULL || fullname_ent==NULL || fullname_ext == NULL)
	{
		if(fullname_ob!=NULL) free(fullname_ob);
		if(fullname_ent!=NULL) free(fullname_ent);
		if(fullname_ext!=NULL) free(fullname_ext);
		printf("Error. There is not enough memory. Can't create file\n");
		return ERROR_FLAG;
	} 
	/*Since the binary encoding is limited to 10 characters, the size of the binary encoding that will be received is at most 2 characters in base 32.
	Therefore we will put \n and \0 in the third and fourth places and leave the first 2 places for the address.*/ 
	code[2]='\n';
	code[3]='\0';
	/*We use ic_code at the beginning to store the number of instructions and the number of data entered.*/
	ic_code[2]='\t';
	ic_code[3]='\0';		
	/*We will copy the name to each of the three files and add the extension according to the file.*/
	strcpy(fullname_ob,name);
	strcpy(fullname_ext,name);
	strcpy(fullname_ent,name);
	strcat(fullname_ob,".ob");
	strcat(fullname_ext,".ext");
	strcat(fullname_ent,".ent");
	/*Open the three files for writing.*/
	file_ob = fopen(fullname_ob, "w+");/*In this file we will write all the captured addresses and their base 32 encoding.*/
	file_ext = fopen(fullname_ext, "w+");/*In this file we will write all the extern labales addresses and their binary encoding.*/
	file_ent = fopen(fullname_ent, "w+");/*In this file we will write all the entry labales addresses and their binary encoding.*/
	
	/*Check if it was successful*/
	if(file_ob==NULL || file_ext==NULL || file_ent == NULL)
	{
		if(file_ent!=NULL) fclose(file_ent);
		if(file_ext!=NULL) fclose(file_ext);
		if(file_ob!=NULL) fclose(file_ob);
		printf("Error.  Can't create file\n");
		return ERROR_FLAG;
	}
	adress = IC;
	/*Calculates the number of instructions entered and represents the number in base 32.*/
	for(i = 1;i>=0 && adress!=0;i--)
	{
		index = adress%32;
		ic_code[i] = base_32[index];
		adress /= 32;
	}
	/*The number of instructions saved. If the number is less than 32 we will give up the ! the first one. If IC = 0, Puts ! in the file*/
	if(IC == 0)
	{
		fputs("!\t", file_ob);
	}
	else
	{
		fputs(ic_code+i+1, file_ob);
	}
	ic_code[2]='\n';
	if(DC == 0)
	{
		fputs("!\n", file_ob);
	}
	else
	{
		adress = DC;
		/*Calculates the number of datas entered and represents the number in base 32.*/
		for(i = 1;i>=0 && DC!=0;i--)
		{
			index = DC%32;
			ic_code[i] = base_32[index];
			DC /= 32;
		}
		/*The number of datas saved. If the number is less than 32 we will give up the ! the first one.*/
		fputs(ic_code+i+1, file_ob);
	}
	/*From here we use ic_code to store the IC*/
	ic_code[2]='\t';
	for(temp_instruction =  head_instruction; temp_instruction != NULL && temp_instruction->next!=NULL; temp_instruction = temp_instruction->next)
	{
		index = temp_instruction->ic%32;
		ic_code[1] = base_32[index];
		index = (temp_instruction->ic/32)%32;
		ic_code[0] = base_32[index];
		fputs(ic_code, file_ob);
		
		
		ic_code[0]='\0';
		strncpy(code1,temp_instruction->binary_code,5);
		code1[5]='\0';
		strncpy(code2,temp_instruction->binary_code+5,5);
		code2[5]='\0';

		code[0] = convert_to_32base(code1);
		code[1] = convert_to_32base(code2);

		fputs(code, file_ob);



	}
	/*Goes through the list of undefined labels and in the process through the list of all labels.
	And if we are indeed talking about the same label, we check if the label is marked as entry or external, then insert the IC and the binary coding into the entry file or the external file, depending on the sign.*/
	for(temp_undefined_lable =  head_undefined_lable; temp_undefined_lable != NULL ; temp_undefined_lable = temp_undefined_lable->next)
	{
		for(temp_lable =  head_lable; temp_lable != NULL; temp_lable = temp_lable->next)
		{
			if(strcmp(temp_lable -> name,temp_undefined_lable->lable_name)==0)
			{
				/*Adding the external to the externals file*/
				if(strcmp(temp_lable -> sign,"external")==0)
				{
					index = (temp_undefined_lable->ic+100)%32;
					ic_code[1] = base_32[index];
					index = ((temp_undefined_lable->ic+100)/32)%32;
					ic_code[0] = base_32[index];

					fputs(temp_lable->name, file_ext);
					fputs("\t", file_ext);
					fputs(ic_code, file_ext);
					fputs("\n", file_ext);
					ic_code[0]='\0';
					externals_counter += 1;/*Increase the externals counter*/
				}
				/*Adding the entry to the entries file*/
				if(strcmp(temp_lable -> sign,"entry")==0)
				{
					adress = temp_undefined_lable->ic+100;
					index = adress%32;
					ic_code[1] = base_32[index];
					index = (adress/32)%32;
					ic_code[0] = base_32[index];

					fputs(temp_lable->name, file_ent);
					fputs("\t", file_ent);
					fputs(ic_code, file_ent);
					fputs("\n", file_ent);
					ic_code[0]='\0';
					entries_counter += 1;/*Increase the entries counter*/		
				}
				break;
			}
		}	
	}
	temp_lable = head_lable; 
	/*In case the label did not enter the undefined labels, but it is still defined as an entry, we will have to add it to the entry file.
	(This cannot happen in an external file)*/
	for(temp_lable =  head_lable; temp_lable != NULL; temp_lable = temp_lable->next)
	{				
		if(strcmp(temp_lable -> sign,"entry")==0)
		{
			adress = temp_lable->adress;

			index = adress%32;
			ic_code[1] = base_32[index];
			index = (adress/32)%32;
			ic_code[0] = base_32[index];

			fputs(temp_lable->name, file_ent);
			fputs("\t", file_ent);
			fputs(ic_code, file_ent);
			fputs("\n", file_ent);
			ic_code[0]='\0';
			entries_counter += 1;/*Increase the entries counter*/		
		}

	}	
	/*Go through the list of data and save the address in "ic_code" and the binary coding in "code", and puts it in the file_ob.*/
	for(temp_data =  head_data; temp_data != NULL && temp_data->next != NULL; temp_data = temp_data->next)
	{
		index = temp_data->dc%32;
		ic_code[1] = base_32[index];
		index = (temp_data->dc/32)%32;
		ic_code[0] = base_32[index];
		fputs(ic_code, file_ob);
		
		strncpy(code1,temp_data->binary_code,5);
		code1[5]='\0';
		strncpy(code2,temp_data->binary_code+5,5);
		code2[5]='\0';
		code[0] = convert_to_32base(code1);
		code[1] = convert_to_32base(code2);
		fputs(code, file_ob);
		ic_code[0]='\0';

	}
	/*If the file_ext is empty, delete it, if not, close it.*/
	if(externals_counter == 0)
		remove(fullname_ext);
	else
		fclose(file_ext);
	/*If the file_ent is empty, delete it, if not, close it.*/
	if(entries_counter == 0)
		remove(fullname_ent);
	else
		fclose(file_ent);
	free(fullname_ob);
	free(fullname_ent);
	free(fullname_ext);
	fclose(file_ob);
	return SUCCESS_FLAG;
}
