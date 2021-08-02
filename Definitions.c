#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PHONE_NUM_LEN 12
#define MIN_LENGTH 6
#include "Declerations.h"
#define COMMAND_ID 0
#define PASS_ID 2
#define USERNAME_ID 1  /* These are the constant numbers  I need for command proccessing*/
#define PHONE_ID 3
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
void UserNameIterator(struct user *head) {
	printf("Below is a list of Users and their usernames :\n");
	if (head->next == NULL) {
		printf("No Users yet ! Hope you are going to become one!\n");
		return;
	}
	else {
		struct user * current = head->next;
	}
	struct user * current = head->next;
	while (current->next != NULL) {
			printf("%s\n",current->username);
			current = current->next;
			
	}
	printf("%s\n", current->username);
	return;
}
void append(User *head, User *appendee)
{

	User *current = head;
	while (current->next != NULL)
	{
		current = current->next;
	}
	current->next = appendee;
	appendee->next = NULL;
}
void appendMessageList(Message *head, Message *appendee) {
	Message *current = head;
	while (current->next != NULL) {
		current = current->next;
	}
	current->next = appendee;
	appendee->next = NULL;
}
char **Tokenizer(char *test, int size,char * delimiter)
{
	char **words;
	int word_count = 1; /* Countwords take tokens */
	for (int i = 0; i < size; i++)
		if (test[i] == delimiter[0])
			word_count++;
	if (word_count == 1)
	{
		words = (char **)malloc(sizeof(char *) * word_count);
		words[0] = (char *)malloc(sizeof(char *) * strlen(test));
		strcpy(words[0], test);
		words[0][strlen(test) - 2] = '\0'; /* A special case of counting the number of words inside the */
		return words;
	}
	else
	{
		int first_difference = 0;
		int difference = 0;
		int last_difference = 0;
		int t = 0;
		int *space_locations = (int *)malloc(sizeof(int) * word_count); /* space is a metaphor for delimiter dont take it too serious*/
		for (int i = 0; i < size; i++)
		{
			if (test[i] == delimiter[0])
			{
				space_locations[t] = i;
				t++;
			}
		}
		for (int i = 0; i < t - 1; i++)
		{
			if (i == 0)
			{
				first_difference = space_locations[i + 1] - space_locations[i];
				last_difference = first_difference;
			}
			else if (i < t - 1)
			{
				difference = space_locations[i + 1] - space_locations[i];
				if (difference > last_difference)
					last_difference = difference;
			}
		}
		words = (char **)malloc(sizeof(char *) * word_count);
		for (int i = 0; i < word_count; i++)
			words[i] = (char *)malloc(sizeof(char) * last_difference);
		for (int i = 0; i < word_count; i++)
		{
			if (i == 0)
				words[i] = strtok(test, delimiter);
			else if (i > 0)
				words[i] = strtok(NULL, delimiter);
		}
		return words;
	}
}
char *GetCommand()
{
	printf(">>"); /* Command Prompt*/
	char c = '\0';
	int t = 1;
	char *input = (char *)malloc(sizeof(char) * t); /*  Malloc and realloc to get any size of input*/
	while (c != '\n')
	{
		c = getchar();
		input[t - 1] = c;
		input = (char *)realloc(input, sizeof(char) * (t + 1));
		t++;
	} /* Recieve the command one by one and Allocate memory for it each time the character is recieved */
	input[t] = '\0' ;
	input[t - 1] = ' ';
	input[t - 2] = ' ';/* These are precautions I did not want the input to have Weird characters at the end*/
	return input;
}
int start(User *head,int *logged_in)
{
	printf("Enter your command\n");
	char *input = GetCommand();
	CommandProcessor(input, strlen(input), head,logged_in);
	return 1;
}
void Register(char *input, User *head_user, char **token_bits, int *logged_in)
{
	if (token_bits[PHONE_ID + 1] != NULL) {
		printf("Command too short check your format \n");
		return;
	}
	if (token_bits[PHONE_ID] == NULL || token_bits[PASS_ID] == NULL || token_bits[PASS_ID + 1] == NULL || token_bits[USERNAME_ID] == NULL) {
		printf("Register command incomplete the format should follow <register> <username>  <pass> <phone> \n");
		return;
	}
	if (*logged_in == 0) {
		int id;
		User* current = (User *)malloc(sizeof(struct user *)); /* Malloc new user and fill them */
		User *new_user = (User *)malloc(sizeof(User));
		int phone_validation = PhoneValidation(token_bits[PHONE_ID], head_user, logged_in, &id); /* Validation Phase */
		int pass_validation = PasswordandValidate(token_bits[PASS_ID]); /* Validation phase */
		int username_validation = ValidateUsername(token_bits[USERNAME_ID], head_user);
		if (phone_validation == 0 || pass_validation == 0 || username_validation == 0) {
			printf("Registeration Unsuccessful try again \n");
			return;
		}
		CreateNewUser(head_user, new_user, token_bits, id); /* Returns a userobject*/
		printf("Registeration successful You are now logged in! \n");
		*logged_in = id;/* We log in the user after registeration . that is the same as changing the ID*/
	}
	else {
		printf("You are already logged please log out \n");
	}
}
void CommandProcessor(char *input, int size, struct user *head, int *logged_in)
{ /* This command Basically Proccess the Entered Command by user and Redirects the user to different parts of the program */
	char **token_bits = Tokenizer(input, size," ");
	if (strcmp(token_bits[COMMAND_ID], "register") == 0) /* Register */
	{
		Register(input, head, token_bits,logged_in);
	}
	else if (strcmp(token_bits[COMMAND_ID], "login") == 0)
	{
		Login(head,token_bits,logged_in);
	}
	else if (strcmp(token_bits[COMMAND_ID], "logout") == 0)
	{
		if (*logged_in != 0) {
			*logged_in = 0;
			printf("You are now logged_out \n");
		}
		else {
			printf("Invalid command  you are not even logged in\n");
		}
	}
	else if (strcmp(token_bits[COMMAND_ID], "send_message") == 0)
	{
		SendMessageP(head, token_bits, logged_in);
	}
	else if (strcmp(token_bits[COMMAND_ID], "see_recieved") == 0)
	{
		ShowMessagesFrom(head, logged_in, token_bits);
	}
	else if (strcmp(token_bits[COMMAND_ID], "see_sent")==0)  {
		SeeSentMessages(head, token_bits,logged_in);
	}
	else if (strcmp(token_bits[COMMAND_ID], "close") == 0)
	{
		write_to_file(head);	/* Save data subroutine should go here*/
		MessageToFile(head);
		exit(0);
	}
	else if (strcmp(token_bits[COMMAND_ID], "info")==0) {
		ShowInfo(head,token_bits);
	}
	else if (strcmp(token_bits[COMMAND_ID], "about")==0) {
		ShowAbout(head, token_bits);
	}
	else if (strcmp(token_bits[COMMAND_ID], "edit_username") == 0) {
		EditUsername(head,logged_in,token_bits);
	}
	else if (strcmp(token_bits[COMMAND_ID], "edit_pass") == 0) {
		Editpassword(head, logged_in, token_bits);
	}
	else
	{
		printf("Command was not recognized try again \n");
	}
}
int PasswordandValidate(char *password)
{
		if (strlen(password)< MIN_LENGTH)
		{
			system("cls");
			
			printf("Password is too short it should be atleast %d letters\n", MIN_LENGTH);
			return 0;
		}
	return 1;
}
int ValidateUsername(char *username, struct user *head) {
	struct user *current = head;
	while (1)
	{
		if (current->next != NULL)
		{
			current = current->next;
			if (strcmp(username, current->username) == 0)
			{ /* Check if the phonenumber has already been registered in the database*/
				system("cls");
				printf("usernames already exists in database try another one\n");
				return 0;
			}
			else {
				continue;
			}
		}
		if (current->next == NULL) {
			break;
		}
	}
	return 1;
}
int PhoneValidation(char *phonenumber, struct user *head,int *logged_in,int *user_rank)
{
	int current_rank = 0;
	struct user *current = head;
	if (phonenumber[0] != '0' || strlen(phonenumber) != PHONE_NUM_LEN - 1)
	{
		system("cls");
		printf("The phone number either did not start with 0 or it was too short or too long (Wrong format anyways)\n");
		return 0;
	}
	for (int i = 0; i < (int)strlen(phonenumber); i++)
	{
		if (phonenumber[i] < 48 || phonenumber[i] > 59)
		{ /* Check for characters that are not digits using ASCI code*/
			system("cls");
			printf("The phone number had characters that were not digits(Wrong format)");
			return 0;
		}
	}
	while (1)
	{
		if (current->next != NULL)
		{
			current = current->next;
			if (strcmp(phonenumber, current->phonenumber) == 0)
			{ /* Check if the phonenumber has already been registered in the database*/
				system("cls");
				printf("Phone already exists in database\n");
				return 0;
			}
			else {
				current_rank++;
				continue;
			}
		}
		if (current->next == NULL) {
			current_rank++;
			*user_rank = current_rank;
			break;
		}
	}
	return 1;
}
void Login(User *head_user, char **token_bits, int *logged_in) {

	if (*logged_in == 0) {
		User* current = head_user;
		if (token_bits[1] == NULL || token_bits[2] == NULL) {
			printf("Login Command Incomplete try again  \n"); /* Validate the command (Being short long ) Missing format */
			return;
		}
		else {
			while (1)
			{
				if (current->next != NULL) {
					current = current->next; /* Iterate throught the users until we reach the username*/
				}
				if (current->username != NULL &&current!=head_user) {
					if (strcmp(token_bits[1], current->username) == 0) /* Compare the Username */
					{
						if (strcmp(token_bits[PASS_ID], current->password) != 0) {
							printf("Wrong password try again \n");
							return;
						}
						else {
							printf("You are now logged in \n");

							*logged_in = current->id; /* Change logged in flag to make it correct*/
							if (*logged_in != 0) {
								ShowMessages(head_user, logged_in);/* We show the messages newly recieved after the user logs in */
							}
							return;
						}
					}
				}
				if (current->next == NULL) {
					printf("User not found");
					return;
				}
			}
		}
	}
	else {
		printf("You are already logged in! ");
	}
}
void CreateNewUser(struct user *head, struct user *new_user, char **token_bits, int id) {
	time_t signup_date;
	time(&signup_date);
	new_user->sign_up_date= ctime(&signup_date); /* Stores sign up date*/
	for (int i = 0; i < (int)(strlen(new_user->sign_up_date)); i++)
		if (new_user->sign_up_date[i] == '\n')
			new_user->sign_up_date[i] = '\0';
	new_user->phonenumber = (char *)malloc(strlen(token_bits[PHONE_ID])); /* Allocate memory and fill user node */
	new_user->phonenumber = token_bits[PHONE_ID];
	new_user->password = (char *)malloc(strlen(token_bits[PASS_ID]));/* Allocate memory and fill user node */
	new_user->password = token_bits[PASS_ID];
	new_user->username = (char *)malloc(strlen(token_bits[USERNAME_ID]));/* Allocate memory and fill user node */
	new_user->username = token_bits[USERNAME_ID];
	new_user->id = id;
	new_user->sent_messages = (struct message *)malloc(sizeof(struct message));
	new_user->recieved_messages = (struct message *)malloc(sizeof(struct message));
	new_user->recieved_messages->next = NULL;
	new_user->sent_messages->next = NULL;
	append(head, new_user); /* Appends a new node user to the linked list of users */
	return;
}
void EditUsername(struct user *head, int *id, char **token_bits) {
	struct user* found;
	if (*id == 0) {
		printf("You are not logged in please try again \n");
		return;
	}
	else {
		if (token_bits[2] != NULL ) {
			printf("Invalid command use ,this command only takes 2 parts the command itself and what you want to change\n");
			return;
		}
		found=Search(head, id);
		if (token_bits[COMMAND_ID + 1] == NULL) {
			printf("Incomplete command check your comamnd's format\n");
			return;
		}
		if (strcmp(token_bits[COMMAND_ID + 1], found->username) == 0) {
			printf("That is already your username try another username \n"); /* Check for the current and the wanted username to not be the same*/
			return;
		}
		else if (ValidateUsername(token_bits[COMMAND_ID+1],head)==1) {
			strcpy(found->username, token_bits[COMMAND_ID + 1]);
			printf("Your username was changed successfully\n");
			return;
		}
	}
}
void Editpassword(struct user *head, int *id, char **token_bits) {
	struct user* found;
	if (*id == 0) {
		printf("You are not logged in please try again \n");
		return;
	}
	else {
		if (token_bits[1] == NULL) {
			printf("Incomplete command check your comamnd's format\n");
			return;
		}
		else {
			if (token_bits[2] != NULL) {
				printf("Invalid command use this command only takes 2 parts the command itself and what you want to change\n");
				return;
			}
			found = Search(head, id);
			if (strcmp(token_bits[PASS_ID - 1], found->password) == 0) {
				printf("Your new password is the same as your last one try again");
				return;
			}
			else if (PasswordandValidate(token_bits[PASS_ID - 1]) == 1) {
				printf("Your password was changed succesfully \n");  /* Notify user */
				strcpy(found->password, token_bits[PASS_ID - 1]); /* Copy the new password inside */
				return;
			}
		}
	}
}
struct user* Search(struct user*head, int *id) {
	struct user * current = head->next;
	/* This Function searches the Database with the ID */
	while (current->id!=*id)
	{
		current = current->next;
	}
	return current;
}
void ShowInfo(struct user *head,char **token_bits) {
	if (token_bits[PASS_ID] != NULL) {
		printf("Invalid use of command please check your format and try again\n");
		return;
	}
	else {
		UserNameIterator(head);
	}
}
void ShowMessagesFrom(struct user* head, int *id,char **token_bits) {
	if (token_bits[PASS_ID - 1] == NULL) {
		printf("Command incomplete \n"); /* The command misses different parts */
		return;
	}
	if (token_bits[PASS_ID] != NULL) {
		printf("Command used  is too long and incorrect try again \n"); /* Command contains more keywords than expected */
		return;
	}
	struct user * self = Search(head, id);
	struct message *start = self->recieved_messages;
	struct user *dest =  SearchByUsername(head, token_bits[PASS_ID - 1]);
	if (dest == NULL) {
		printf("Username entered does not belong to an existing User please try again \n"); /* The username is not found at the destination */
		return;
	}
	if (start->next == NULL) {
		printf("No  messages even exist \n");
		return;
	}
	else {
		start = start->next;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); /* Introduce a new console to change color */
		while (1) {
			if ( start->sender==dest ) {
				if (start->seen == 0) {
					SetConsoleTextAttribute(hConsole,
						FOREGROUND_GREEN); /* set Console text color to green to identify unseen from seen messages*/
					printf("message sent on date %s from %s  message text : %s \n", start->sent_date, start->sender->username, start->message_text);
					SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED); /* Changes the color back (reverts it to white )*/
					start->seen = 1; /* The message is seen now so we change the message status*/
				}
				else
				{
					printf("message sent on date %s from %s  message text : %s \n", start->sent_date, start->sender->username, start->message_text);
				}
			}
			if (start->next != NULL) {
				start = start->next;
			}
			else {
				printf("End of Messages\n");
				return;
			}
		}
	}
}
void ShowMessages(struct user*head,int *id) {
	struct user * self = Search(head, id);
	struct message *start = self->recieved_messages;
	if (start == NULL) {
		printf("No new Messages \n");
		return;
	}
	if (start->next == NULL) {
		printf("No  messages \n");
		return;
	}
	else {
		start = start->next;
		while (1) {
			if (start->seen == 0) {
				printf(" new message sent on date %s from %s \n", start->sent_date, start->sender->username);
			}
				if (start->next != NULL) {
					start = start->next;
				}
				else{
					printf("End of messages \n");
					return;
				}
		}
	}
}
void ShowAbout(struct user *head, char **token_bits) {
	if (head->next == NULL) {
		printf("No users yet please try again later \n");/* Logically the user shouldnt be able to view the messages of  NULL People !!*/
		return;
	}
	else {
		int userfound = 0;
		struct user *current = head->next;
		if (token_bits[PASS_ID] != NULL) {
			printf("Invalid use of command please check your format and try again\n"); /*  I basically check if the further compartments of the command are NULL or not if they are null it is currect and if they are not well :|*/
			return;
		}
		else {
			while (1)
			{
				if (strcmp(current->username, token_bits[COMMAND_ID+1]) == 0) {
					printf("======\n Phonenumber: %s\n ===== \n Date Joined :%s\n =====  \n Username : %s  \n ===== \n", current->phonenumber,current->sign_up_date,current->username);
					userfound = 1;
					return;
				}
				if (current->next != NULL) {
					current = current->next; /* Skip to next node*/
				}
				else {
					break;
				}
			}
			if (userfound == 0) {
				printf("The username you entered was not found make sure you have the right username and try again\n");
			}
		}
	}
}
struct user * SearchByUsername(User *head, char *input) {
	/* I have different Search functions they all take different types for example this function searches the User Linked list with the username*/
	User * current = head->next;
	while (1) {
		if (strcmp(current->username, input) == 0) {
			return current;
		}
		else {
			if (current->next != NULL) {
				current = current->next;
			}
			else {
				break;
			}
		}
	}
	return NULL;
}
void write_to_file(struct user*head) {
	FILE *user_data=fopen("database.txt", "w+");
	if (head->next != NULL) {
		User * current = head->next; /* Starting Node */
		while (1) {/* This writes User Data to file */
			fprintf(user_data,",%s,%s,%s,%s,%d,", current->username, current->password, current->phonenumber, current->sign_up_date, current->id);
			if (current->next != NULL) {
				current = current->next;
			}
			else {
				break;
			}
		}
	}
	fclose(user_data);
}
void SeeSentMessages(User *head, char **token_bits, int *logged_in) {
	if (token_bits[PASS_ID - 1] == NULL) {/* We basically check for faults in the command the first part might be right but the other parts of the commands might be missing or wrong if they are wrong we alert user*/
		printf("Incomplete Command please try again \n");
		return;
	}
	if (token_bits[PASS_ID] != NULL) {
		printf("Command is invalid because it is too long please check command format\n");
		return;
	}
	else {
		struct user * reciever = SearchByUsername(head, token_bits[PASS_ID - 1]);
		struct user *self = Search(head, logged_in);
		struct message * start = self->sent_messages;
		if (self->sent_messages->next==NULL){
			printf("No messages  were sent \n");
		}
		else {
			while (1) {
				if (start->next == NULL) {
					printf("End of messages \n"); /*We announce the end for the user to know when to exit or not expect messages */
					return;
				}
				else
				{ 
					start = start->next;
					if (strcmp(start->reciever->username,token_bits[PASS_ID-1]) == 0) { /* Printing the message content in one line*/
						printf("message sent by you on  %s to %s  : text  :%s \n",start->sent_date,start->reciever->username,start->message_text );
					}
				}
			}
		}
	}
}
void SendMessageP(User *head,char **token_bits,int *logged_in) {
	struct user * found;
	struct user *self_found;
	struct message *new_message;
	if (*logged_in == 0) {
		printf("You are not logged in please try again logged in \n"); /* WE CHECK to see if the user is logged in or not */
		return;
	}
	if (token_bits[PASS_ID] == NULL) {
		printf("Command incomplete try again \n");
		return;
	}
	if (token_bits[PASS_ID + 1] != NULL) {
		printf("Command too long try again  \n");
		return;
	}
	else {
		found = SearchByUsername(head, token_bits[PASS_ID-1]);
		self_found = Search(head, logged_in);
		if (found!=NULL) {/* I use reccouring PASSID because it represensts two in this exception I just use it to have not used const numbers in my code*/
			new_message=CreateMessage(self_found, found, token_bits[PASS_ID]);
			appendMessageList(found->recieved_messages,new_message);
			appendMessageList(self_found->sent_messages, new_message);
			printf("Message sent successfuly \n");
		}
		else {
			printf("The Username entered does not exist please try again\n"); /* Alert user*/
			return;
		}
	}
}
struct message * CreateMessage(struct user * sender,struct user * reciever,char *text) {
	Message * new_messsge = (Message *)malloc(sizeof(Message));
	time_t signup_date;/* This function creates messages and returns them It sets sennder and reciever and generates a random Increasing ID using a time Seed that goes forward*/
	time(&signup_date);
	new_messsge->sent_date = ctime(&signup_date); /* Stores sign up date*/
	for (int i = 0; i < (int )(strlen(new_messsge->sent_date)); i++)
		if (new_messsge->sent_date[i] == '\n')
			new_messsge->sent_date[i] = '\0';
	new_messsge->sender = sender;
	new_messsge->reciever = reciever;
	new_messsge->message_text = (char *)malloc(strlen(text));
	strcpy(new_messsge->message_text, text);
	new_messsge->seen = 0;
	srand((unsigned )time(0));
	new_messsge->unique_id = (long long int)rand();
	new_messsge->next = NULL; /* We prepare the messages objects them selves as list nodes*/
	return new_messsge;
}
void ReadFromFile(User *head) {
	FILE *open = fopen("database.txt", "r");
	if (open == NULL) {
		return;
	}
	char *stream;
	char c = fgetc(open);
	fseek(open, 0, SEEK_END);
	int buffer_size = ftell(open);
	fseek(open, 0, SEEK_SET);

	stream = (char *)malloc(buffer_size * sizeof(char));
	stream[0] = c;
	int t = 0;
	int a = 0;
	if (buffer_size !=0) {
		while (t < buffer_size)
		{
			stream[t] = fgetc(open);
			t++;
		}
		stream[t] = '\0';
		char **data = Tokenizer(stream, strlen(stream), ",");
		t = 0;
		while (data[t] != NULL) {	/* We tokenize the data and since the data has an order to it we can use the remenant operator to determine which Part should go where for exames indexes that %6 equal 1 are used for accountname*/
			if (t % 5 == 0 && t!=0) {
				FillUser(head, data,a);
				a++;
			}
			t++;
		}
			FillUser(head, data, a); 
	}
	else
	{
		return;
	}
	
}
void MessageToFile(User *head) {
	User *current = head;
	FILE *messages = fopen("messages.txt","w");
	Message *message_cur;
	if (head->next == NULL) {
		return;
	}
	else {
		current = current->next;
		while (1)
		{
			if (current != NULL) {
				message_cur = current->sent_messages;

				if (message_cur->next != NULL) {
					message_cur = message_cur->next;
				}
				else
				{
					current = current->next;
					continue;
				}
			}
			else
			{
				break;
			}
			while (1)
			{
				if (message_cur!=NULL) /* Writes the infomration of messages to the file seperated by comma values*/
					fprintf(messages,",%s,%s,%s,%lld,%s,%d,",message_cur->message_text,message_cur->reciever->username, message_cur->sender->username,message_cur->unique_id, message_cur->sent_date, message_cur->seen);
				if (message_cur->next != NULL) {
					message_cur = message_cur->next;
				}
				else
				{
					break;
				}
			}
			if (current->next != NULL) {
				current = current->next;
			}
			else
			{
				break;
			}
		}
	}
	fclose(messages);
}
void FillUser(User *head,char **data,int a) {

	User *fill = (User *)malloc(sizeof(User)); /* We basically create a User Object and fill it with the info recieved from the file*/
	fill->id = atoi(data[5*a+FOUR]);
	fill->username = data[5 * a + COMMAND_ID];
	fill->password = data[5 * a + ONE];
	fill->sign_up_date = data[5 * a + THREE];
	fill->phonenumber = data[5 * a + TWO];
	fill->recieved_messages = (struct message *)malloc(sizeof(Message ));
	fill->sent_messages = (struct message *)malloc(sizeof(Message ));
	fill->recieved_messages->next = NULL;
	fill->sent_messages->next = NULL;
	append(head, fill);
}
void FiletoMessage(User *head) {
	FILE *messages = fopen("messages.txt", "r");
	if (messages == NULL) {
		return;
	}
	char *stream;
	char c = fgetc(messages);
	char **data;
	fseek(messages, 0, SEEK_END);
	int buffer_size = ftell(messages);
	fseek(messages, 0, SEEK_SET);
	stream = (char *)malloc(buffer_size * sizeof(char));
	stream[0] = c;
	int t = 0;
	int a = 0;
	if (buffer_size != 0) {
		while (t < buffer_size)
		{
			stream[t] = fgetc(messages); /* Read from file stream one by one until we reach the end*/
			t++;
		}
		stream[t] = '\0';
		data = Tokenizer(stream, strlen(stream), ","); /* We serialize the data and turn them into little bits of info seperated by commas*/
	}
	else
	{
		return;
	}
	t = 0;
	while (data[t] != NULL) {
		if (t % 6 == 0 && t != 0) {
			FillMessages(head, a, data); /* This basically creates Objects of type Message and fills them up with the information read from the file*/
			a++;
		}
		t++;
	}
	FillMessages(head, a, data);
	fclose(messages);
}
void FillMessages(User *head,int a,char **data) {
	Message *new_message = (Message *)malloc(sizeof(User)); /*Allocate a new user and fill it with data from the file */
	User *reciever= SearchByUsername(head, data[6 * a + ONE]);
	User *sender = SearchByUsername(head, data[6 * a + TWO]);
	new_message->reciever =reciever;
	new_message->sender = sender;
	new_message->message_text = data[6 * a + COMMAND_ID]; /* Command ID is the same as zero this is me just being lazy with naming my Macros*/
	new_message->unique_id =atoll(data[6 * a + THREE]);
	new_message->sent_date = data[6 * a + FOUR];
	new_message->seen = atoi(data[6 * a + FOUR + 1]);
	new_message->next = NULL;
	appendMessageList(sender->sent_messages, new_message);/* Append to sent*/
	appendMessageList(reciever->recieved_messages, new_message);/*append to recieved */
}