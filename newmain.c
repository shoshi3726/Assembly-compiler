
#include "transition1.h"
#include "macro.h"

/*Main function which starts the macro process and if success. Keep processing the assembler*/
int main(int argc, char *argv[])
{
	
	int i,result;
	printf("Hello. Welcome to our assembler.\n");
	/*Read the file name from the user and make a macro transition, if there are no errors in the macro we will go to the first transition.*/
	for(i=1;i<argc;i++)
	{
		printf("Assembler start processing with file %s\n",argv[i]);
		result = open_macro(argv[i]); 
		if(result != ERROR_FLAG)
			result = first_transition(argv[i]);
		else
			printf("Assembler faild in macro process.\nFinish process to %s file.\n",argv[i]);
		if(result != ERROR_FLAG)
			printf("Assmbler completed successfully for file %s.\n",argv[i]);
		else
			printf("Assembler faild.\nFinish process to %s file.\n",argv[i]);
	}
	printf("Thanks for using Assembler.\nHope you enjoyed the process!\n");
	return result;
}




