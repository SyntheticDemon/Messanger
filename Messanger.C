#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PHONE_NUM_LEN 12
#include "Declerations.h"
int main()
{
	int logged_in_flag = 0; /* A flag Intitially set to zero that changes to the ID of the user when someone logs in */
	User *head = (User *)malloc(sizeof(User)); /* Make dummy Node*/
	head->next = NULL;
	printf("Welcome to UT messanger this application is command based\n");
	ReadFromFile(head); /* We read from the first file of User Account Data first*/
	FiletoMessage(head); /* THe we read the messages from file */
	while (1)
	{
			start(head,&logged_in_flag);
			
	}
	write_to_file(head);
	return 0;
}
