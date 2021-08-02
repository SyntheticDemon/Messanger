#pragma once
#include <time.h>
typedef struct message
{
	int seen;
	long long int  unique_id;
	char *sent_date;
	char *message_text;
	struct user * sender;
	struct user * reciever;
	struct message *next;
} Message;
typedef struct user
{
	int id;
	char *username;
	char *password;
	char *phonenumber;
	char *sign_up_date;
	struct message*recieved_messages;
	struct message *sent_messages;
	struct user *next;
} User;
void Register(char *input, User *head_user, char **token_bits,int *logged_in);
void CommandProcessor(char *input, int size, struct user *head,int *logged_in);
void UserNameIterator(struct user *head);
void append(User *head, User *appendee);
char **Tokenizer(char *test, int size,char* delimiter);
char *GetCommand();
int start(User *head,int *logged_in);
int PhoneValidation(char *phonenumber, struct user *head, int *logged_in, int *user_rank);
int PasswordandValidate(char *password);
int ValidateUsername(char *username, struct user *head);
void Login(User *head_user, char **token_bits, int *logged_in);
void CreateNewUser(struct user *head, struct user *new_user, char **token_bits, int id);
void EditUsername(struct user *head, int *id,char **token_bits);
void Editpassword(struct user *head, int *id, char **token_bits);
struct user* Search(struct user*head, int *id);
void ShowInfo(struct user *head, char **token_bits);
void UserNameIterator(head);
void ShowAbout(head, token_bits);
void write_to_file(struct user*head);
void SendMessageP(User *head, char **token_bits, int *logged_in);
struct user * SearchByUsername(User *head, char *input);
struct message * CreateMessage(struct user * sender, struct user * reciever, char *text);
void ShowMessages(struct user*head, int *id);
void ShowMessagesFrom(struct user* head, int *id, char **token_bits);
void SeeSentMessages(User *head, char **token_bits, int *logged_in);
void ReadFromFile(User *head);
void FillUser(User *head, char **data,int a);
void MessageToFile(User *head);
void FiletoMessage(User *head);
void FillMessages(User *head,int a,char **data);