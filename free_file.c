
#include "transition1.h"

/*Gets head of instruction list and free the memory wich allocated*/
int free_all_instructions(instruction * head)
{
	instruction * temp = head;
	if(head == NULL)
		return 0;
	while(head!=NULL)
	{
		temp = temp->next;
		free(head);
		head = temp;

	}
	return 0;
}

/*Gets head of data list and free the memory wich allocated*/
int free_all_data(data * head)
{
	data * temp = head;
	while(head!=NULL)
	{
		temp = temp->next;
		free(head);
		head = temp;

	}
	return 0;
}

/*Gets head of lables list and free the memory wich allocated*/
int free_all_lables(lable * head)
{
	lable * temp = head;
	while(head!=NULL)
	{
		temp = temp->next;
		free(head);
		head = temp;

	}
	return 0;
}

/*Gets head of undefined_lable list and free the memory wich allocated*/
int free_all_undefined_lables(undefined_lable * head)
{
	undefined_lable * temp = head;
	while(head!=NULL)
	{
		temp = temp->next;
		free(head);
		head = temp;

	}
	return 0;
}
