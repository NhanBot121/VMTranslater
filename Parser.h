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


#define MAX_COMMAND_LENGTH 128
#define MAX_NUM_COMMAND 1024

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
	CommandType type;
} Command;

typedef struct __Parser {
	Command commands[MAX_NUM_COMMAND];
	int current;
	int count;
} Parser;

Parser* parser_init(char* input_file);
int hasMoreLines(Parser* parser);
void advance(Parser* parser);
CommandType commandType(Parser* parser);
const char* arg1(Parser* parser);
int arg2(Parser* parser);

#endif
