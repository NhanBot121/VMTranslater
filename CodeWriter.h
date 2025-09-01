#ifndef CODEWRITER_H
#define CODEWRITER_H
#include <stdlib.h>
#include <stdio.h>
// get the name of the output file
char* create_output_filename(const char* input_filename);


// 
void writeArithmetic(FILE* fp, const char* cmd);

//
void writePushPop(FILE* fp, const char* seg, int idx);

#endif
