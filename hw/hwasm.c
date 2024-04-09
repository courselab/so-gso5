/**
 * Code created by Felipe Tavoni
 * A parser to convert an AT&T "Hello World" code into Machine code.
 * As per requirements, it could be a "specific-purpose implementation that works only with the given input."
 *  There might be some enhancements that can be done in order to provide a better generic parsing.
 */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_LINE_LENGTH 100

unsigned char  extractHex(const char *instruction) {

    const char *ptr = strchr(instruction, '$');
    unsigned int value;
    sscanf(ptr + 1, "%x", &value);
    return value;
}

void convertInstruction(const char *assemblyLine, FILE* output) {

    unsigned char instruction[2];

    /*
     * MOVB instruction
     */
    if (strncmp(assemblyLine, "movb", 4) == 0) {
        instruction[1] = extractHex(assemblyLine);
        if (strncmp(assemblyLine + 11, "%ah", 3) == 0) {
            instruction[0] = 0xb4;
        }
        fwrite(instruction, sizeof(unsigned char), 2, output);
    }

    /*
     * MOV instruction
     */
    if (strncmp(assemblyLine, "mov ", 4) == 0) {
        instruction[1] = assemblyLine[6];
        if (strncmp(assemblyLine + 10, "%al", 3) == 0) {
            instruction[0] = 0xb0;
        }
        fwrite(instruction, sizeof(unsigned char), 2, output);
    }

    /*
     * INT instruction
     */
    if (strncmp(assemblyLine, "int", 3) == 0) {
        instruction[0] = 0xcd;
        instruction[1] = extractHex(assemblyLine);
        fwrite(instruction, sizeof(unsigned char), 2, output);
    }

    /*
     * JMP
     */
    if (strncmp(assemblyLine, "jmp", 3) == 0) {
        instruction[0] = 0xeb;
        /* TODO: Automate this relative detection. */
        instruction[1] = 0xfd; /* Step -3 bytes, to reach halt function. It is -3 in complements two (1111 1101), converted into hex (0xFD)*/
        fwrite(instruction, sizeof(unsigned char), 2, output);
    }

    /*
     * HLT instruction
     */
    if (strncmp(assemblyLine, "hlt", 3) == 0) {
        instruction[0] = 0xf4;
        fwrite(&instruction[0], sizeof(unsigned char), 1, output);
    }

    /*
     * FILL instruction
     */
    if (strncmp(assemblyLine, ".fill", 5) == 0) {
        int i;
        long zeros;
        sscanf(assemblyLine, "%*[^0-9]%ld", &zeros);
        int toFill = zeros - ftell(output);

        unsigned char zero = 0x0;
        for (i = 0; i < toFill; i++) { /* Fill with 0´s, subtracting already fulfilled bytes */
            fwrite(&zero, sizeof(unsigned char), 1, output);
        }
    }

    /*
     * WORD instruction
     */
    if (strncmp(assemblyLine, ".word", 5) == 0) {
        instruction[0] = 0x55;
        instruction[1] = 0xaa;
        fwrite(instruction, sizeof(unsigned char), 2, output);
    }
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "wb");
    if (outputFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    char *line = (char *) malloc(MAX_LINE_LENGTH * sizeof(char));
    while (fgets(line, MAX_LINE_LENGTH * sizeof(char), inputFile) != NULL) {
        if ((strncmp(line, ".code", 5) != 0) || (line[0] != '#')) {
            convertInstruction(line, outputFile);
        }
    }

    fclose(inputFile);
    fclose(outputFile);
    return 0;
}