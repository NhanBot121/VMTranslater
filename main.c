#include "Parser.h"
#include "CodeWriter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

int main(int argc, char* argv[]) {
	if (argc != 2) {
		return 1;
	}

	printf("in main\n");

	Parser* parser = malloc(sizeof(Parser));
	parser = parser_init(argv[1]);

	// create ouput file 
	const char* output_file = output_filename(argv[1]);
	FILE* ofp = fopen(output_file, "w");

	while (hasMoreLines(parser)) {
		// set the current command
		advance(parser);

		char cur_cmd[MAX_COMMAND_LENGTH];
		cur_cmd[0] = '\0';

		strcpy(cur_cmd, commandLine(parser));

		char arg1[7];
		arg1[0] = '\0';
		strcpy(arg1, get_arg1(cur_cmd));

		int arg2 = 0;

		if (commandType(cur_cmd) == C_PUSH || commandType(cur_cmd) == C_POP) {
			arg2 = get_arg2(cur_cmd);

			writePushPop(ofp, arg1, arg2); 
		} else if (commandType(cur_cmd) == C_ARITHMETIC) {
			writeArithmetic(ofp, arg1);	
		}
	}

	return 1;

}
