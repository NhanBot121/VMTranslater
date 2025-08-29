#ifndef PARSER_H
#define PARSER_H


/*
* This modulde handles the parsing of a single .vm file
*
* The Parser provides services for reading a VM commnand
* unpacking the command into its various components,
* and providing convenient access to these compoents.
*
* Parser ignores all white spaces and comments.
*
*/


#define MAX_COMMAND_LENGTH 256
#define MAX_NUM_COMMAND 4096

typedef enum __CommandType {
	C_ARITHMETIC,
	C_PUSH, C_POP,
	C_GOTO, C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL,
} CommandType;

typedef struct __Command {
	char line[MAX_COMMAND_LENGTH];
} Command;

typedef struct __Parser {
	Command commands[MAX_NUM_COMMAND];
	int current;
	int count;
} Parser;

Parser* parser_init(const char* input_file);
int hasMoreLines(Parser* parser);
void advance(Parser* parser);
CommandType commandType(const char*);
char* commandLine(Parser* parser);
char* get_arg1(char*);
int get_arg2(char*);

// Some helper functions
int isEmptyLine(const char* line);
int isCommentLine(const char* line);
void trimSpaces(char* line);
#endif
