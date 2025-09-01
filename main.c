#include "Parser.h"
#include "CodeWriter.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
	if (argc != 2) {
		return 1;
	}

	printf("infile = %s \n", argv[1]);

	const char* input_file = argv[1];

	Parser* parser = malloc(sizeof(Parser));
	parser = parser_init(argv[1]);

	char ofn[100];
	ofn[0] = '\0';

	strcpy(ofn, create_output_filename(input_file));

	printf("outfine = %s \n", ofn);

	FILE* ofp = fopen(ofn, "w");

	// create ouput file 

	char cmd[MAX_COMMAND_LENGTH];
	cmd[0] = '\0';

	while (hasMoreLines(parser)) {
		// set the current command
		advance(parser);

		strcpy(cmd, commandLine(parser));

		printf("%s\n", commandLine(parser));

		char* arg1;
		arg1 = get_arg1(commandLine(parser));

		int arg2 = 0;

		CommandType type = commandType(commandLine(parser));

		if (type == C_PUSH || type == C_POP) {
			arg2 = get_arg2(commandLine(parser));

			writePushPop(ofp, arg1, arg2); 
		} else if (type == C_ARITHMETIC) {
			writeArithmetic(ofp, arg1);	
		}
	}

	fclose(ofp);

	return 1;

}
