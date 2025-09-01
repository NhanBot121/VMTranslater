#include "Parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser* parser_init(const char* input_file) {
	/*
	* Open the inpute file and get ready to parse it.
	*
	* IN: an .vm file
	* OUT: a Parser object contain an array of commands
	* 	and the number of commands
	*
	* For each command, extract its command line
	* and type, discard white spaces 
	*/

	// Open the file 
	FILE* fp = fopen(input_file, "r");

	if (!fp) {
		fprintf(stderr, "faild to open file\n");
		exit(1);
	}

	// allocate the parser
	Parser* parser = malloc(sizeof(Parser));
	if (!parser) {
		fprintf(stderr, "failed to allocate memory for parser\n");
		exit(1);
	}
	parser->count = 0;
	parser->current = -1 ;


	// Read its lines
	char line[MAX_COMMAND_LENGTH];
	while(fgets(line, sizeof(line), fp)) {

		line[strcspn(line, "\n")] = '\0'; // turn "\n" to '\0'
		// Skip comment and empty line
		if (isCommentLine(line) || isEmptyLine(line)) {
			continue;
		}
		// Trim white spaces
		trimSpaces(line);

		Command cmd = parser->commands[parser->count];

		// Write the cmd line and its type
		strcpy(cmd.line, line);
		//
		//
		
		parser->commands[parser->count++] = cmd;
	}

	fclose(fp);
	return parser;
}

int hasMoreLines(Parser* parser) {
	if (parser-> current < parser->count) {
		return 1;
	} else {
		return 0;
	}
}

void advance(Parser *parser) {
	/*
	 * Only called if hasMoreLines is true
	 * initially there's no cmd -> current = -1 at the beginning
	 */
	parser->current++;
}

char* commandLine(Parser* parser) {
	/*
	 * Return the current command line
	 */
	return parser->commands[parser->current].line;
}


CommandType commandType(const char* line) {
    // Skip leading whitespace
    while (*line == ' ' || *line == '\t') {
        line++;
    }
    
    // Extract the first word (command)
    char command[20];
    int i = 0;
    while (*line != ' ' && *line != '\t' && *line != '\0' && *line != '\n' && i < 19) {
        command[i++] = *line++;
    }
    command[i] = '\0';
    
    // Check for arithmetic commands
    if (strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 || 
        strcmp(command, "neg") == 0 || strcmp(command, "eq") == 0 || 
        strcmp(command, "gt") == 0 || strcmp(command, "lt") == 0 || 
        strcmp(command, "and") == 0 || strcmp(command, "or") == 0 || 
        strcmp(command, "not") == 0) {
        return C_ARITHMETIC;
    }
    
    // Check for memory access commands
    if (strcmp(command, "push") == 0) {
        return C_PUSH;
    }
    
    if (strcmp(command, "pop") == 0) {
        return C_POP;
    }
/*   
    // Check for program flow commands (for later projects)
    if (strcmp(command, "label") == 0) {
        return C_LABEL;
    }
    
    if (strcmp(command, "goto") == 0) {
        return C_GOTO;
    }
    
    if (strcmp(command, "if-goto") == 0) {
        return C_IF;
    }
    
    // Check for function commands (for later projects)
    if (strcmp(command, "function") == 0) {
        return C_FUNCTION;
    }
    
    if (strcmp(command, "call") == 0) {
        return C_CALL;
    }
    
    if (strcmp(command, "return") == 0) {
        return C_RETURN;
    }
*/  
    // This should not happen with valid VM code
    // You might want to handle this case based on your error handling strategy
    return C_ARITHMETIC; // or throw an error
}
char* get_arg1(char* line) {
	if (commandType(line) == C_ARITHMETIC) {
		return line;
	} else {
		static char arg1[7];
		arg1[0] = '\0';

		char* space_pos = strstr(line, " "); 
		char* start_pos = space_pos + 1;
		int arg1_len;
		for (int i = 0; ; i++) {
			if (!isspace(start_pos[i])) {
				arg1[i] =start_pos[i];
			} else {
				arg1_len = i + 1;
				break;
			}
		}

		arg1[arg1_len] = '\0';

		
		return arg1;

	}
}

int get_arg2(char* line) {

	char* space_pos_1st = strstr(line, " ");
	char* space_pos_2nd = strstr(space_pos_1st+1, " ");

	// arg2 is in between 2nd and 3rd space
	char* start_pos = space_pos_2nd + 1;
	int arg2;
	sscanf(start_pos, "%d", &arg2);

	return arg2;

}

// Some helper functions
int isEmptyLine(const char* line) {
	if (strlen(line) == 0) {
		return 1;
	} else {
		for (int i = 0; i < strlen(line); i++) {
	
			if (!isspace(line[i])) {
				return 0;
			}
		}
	}

	return 1;
}

int isCommentLine(const char* line) {
	char *cmt = strstr(line, "//");

	if(cmt) {
		return 1;
	} else {
		return 0;
	}
}

void trimSpaces(char* line) {
	int len = strlen(line);

	int start = 0;
	int end = len - 1;

	// trim leading white spaces
	while (isspace(line[start]) && start < end) {
		start++;
	}

	// trim trailing white spaces
	while (isspace(line[end]) && end > start) {
		end--;
	}

	int new_len = end - start + 1;
	memmove(line, line+start, new_len);

	line[new_len] = '\0';
}
