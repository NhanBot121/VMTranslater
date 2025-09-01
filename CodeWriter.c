#include "CodeWriter.h"
#include <string.h>
// Create output filename from input filename
char* create_output_filename(const char* input_filename) {
    static char output_filename[256];
    
    // Copy input filename
    strcpy(output_filename, input_filename);
    
    // Find the last dot for extension
    char* dot_pos = strrchr(output_filename, '.');
    
    if (dot_pos) {
        // Replace extension with .hack
        strcpy(dot_pos, ".asm");
    } else {
        // No extension, just append .hack
        strcat(output_filename, ".asm");
    }
    
    return output_filename;
}


int label_counter = 0;

// Function to write push/pop commands
void writePushPop(FILE* fp, const char* seg, int idx) {
    if (strcmp(seg, "push") == 0) {
        // This shouldn't happen - seg should be the memory segment
        return;
    }
    
    // Push operations
    if (strstr(seg, "constant") != NULL) {
        // push constant i
        fprintf(fp, "@%d\n", idx);
        fprintf(fp, "D=A\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M\n");
        fprintf(fp, "M=D\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "M=M+1\n");
    }
    else if (strstr(seg, "local") != NULL) {
        // push/pop local i
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@LCL\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "A=D+A\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@LCL\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "D=D+A\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "argument") != NULL) {
        // push/pop argument i
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@ARG\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "A=D+A\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@ARG\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "D=D+A\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "this") != NULL) {
        // push/pop this i
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@THIS\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "A=D+A\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@THIS\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "D=D+A\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "that") != NULL) {
        // push/pop that i
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@THAT\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "A=D+A\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@THAT\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@%d\n", idx);
            fprintf(fp, "D=D+A\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@R13\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "temp") != NULL) {
        // push/pop temp i (temp segment starts at RAM[5])
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@R%d\n", 5 + idx);
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@R%d\n", 5 + idx);
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "pointer") != NULL) {
        // push/pop pointer i (0=THIS, 1=THAT)
        if (strstr(seg, "push") != NULL) {
            if (idx == 0) {
                fprintf(fp, "@THIS\n");
            } else {
                fprintf(fp, "@THAT\n");
            }
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            if (idx == 0) {
                fprintf(fp, "@THIS\n");
            } else {
                fprintf(fp, "@THAT\n");
            }
            fprintf(fp, "M=D\n");
        }
    }
    else if (strstr(seg, "static") != NULL) {
        // push/pop static i
        if (strstr(seg, "push") != NULL) {
            fprintf(fp, "@Static.%d\n", idx);
            fprintf(fp, "D=M\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "A=M\n");
            fprintf(fp, "M=D\n");
            fprintf(fp, "@SP\n");
            fprintf(fp, "M=M+1\n");
        } else { // pop
            fprintf(fp, "@SP\n");
            fprintf(fp, "AM=M-1\n");
            fprintf(fp, "D=M\n");
            fprintf(fp, "@Static.%d\n", idx);
            fprintf(fp, "M=D\n");
        }
    }
}

// Function to write arithmetic commands
void writeArithmetic(FILE* fp, const char* cmd) {
    if (strcmp(cmd, "add") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D+M\n");
    }
    else if (strcmp(cmd, "sub") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=M-D\n");
    }
    else if (strcmp(cmd, "neg") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=-M\n");
    }
    else if (strcmp(cmd, "eq") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@EQ_TRUE_%d\n", label_counter);
        fprintf(fp, "D;JEQ\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@EQ_END_%d\n", label_counter);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(EQ_TRUE_%d)\n", label_counter);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(EQ_END_%d)\n", label_counter);
        label_counter++;
    }
    else if (strcmp(cmd, "gt") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@GT_TRUE_%d\n", label_counter);
        fprintf(fp, "D;JGT\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@GT_END_%d\n", label_counter);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(GT_TRUE_%d)\n", label_counter);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(GT_END_%d)\n", label_counter);
        label_counter++;
    }
    else if (strcmp(cmd, "lt") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "D=M-D\n");
        fprintf(fp, "@LT_TRUE_%d\n", label_counter);
        fprintf(fp, "D;JLT\n");
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=0\n");
        fprintf(fp, "@LT_END_%d\n", label_counter);
        fprintf(fp, "0;JMP\n");
        fprintf(fp, "(LT_TRUE_%d)\n", label_counter);
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=-1\n");
        fprintf(fp, "(LT_END_%d)\n", label_counter);
        label_counter++;
    }
    else if (strcmp(cmd, "and") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D&M\n");
    }
    else if (strcmp(cmd, "or") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "AM=M-1\n");
        fprintf(fp, "D=M\n");
        fprintf(fp, "A=A-1\n");
        fprintf(fp, "M=D|M\n");
    }
    else if (strcmp(cmd, "not") == 0) {
        fprintf(fp, "@SP\n");
        fprintf(fp, "A=M-1\n");
        fprintf(fp, "M=!M\n");
    }
}
