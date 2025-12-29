#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct {
    char main_file[100]
} Information;

long getFileSize(char filename[]);
void parseArguments(int argc, char *argv[], Information *information);
void openFile(long size, char filename[], char *text[]);

int main(int argc, char *argv[]) {
    Information information;
    printf("Ahora ejecutando...\n");
    parseArguments(argc, argv, &information);
    printf("argc: %d\n", argc);
    long size = getFileSize(information.main_file);
    printf("size: %ld\nname: %s\n", size, information.main_file);
    char *fc = calloc(size, 1);
    if (fc == NULL) {
        printf("ERR: NOMEM");
        exit(1);
    }
    openFile(size, information.main_file, fc);
    printf("fc:\n%s\n", fc);
    
    // At the end of program, free any remaining allocated memory
    free(fc); fc = NULL;
    return 0;
}

/*
*  Indicates the size in bytes of a file
*  Pass a char array of the filename
*/
long getFileSize(char filename[]) {
    long size;
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("Archivo inválido: %s", filename);
        exit(1);
    }
    
    if (fseek(f, 0L, SEEK_END) == 0) {
        size = ftell(f);
        if (size == -1L) {
            perror("Error al revisar posición del archivo");
            exit(1);
        }
    } else {
        perror("Error al encontrar fin del archivo");
        exit(1);
    }
    
    fclose(f);
    return size;
}

void parseArguments(int argc, char *argv[], Information *information) {
    int specifying = 0; // Specifying main file
    for (int i = 1; i < argc; i++) { // skip argv[0]
        if (specifying == 0 && argv[i][0] != '-') {
            strcpy(information->main_file, argv[i]);
        }
    }
};

void openFile(long size, char filename[], char *text[]) {
    const char newline_str[] = "\n";
    char *line;
    int isFirst = 1;
    int size_int = (size < INT_MAX) ? ((size >= 0) ? (int) size : 0) : INT_MAX; // Must be positive number
    line = calloc(size, 1);
    if (line == NULL) {
        printf("ERR: ENOMEM");
        exit(1);
    }
    FILE *f = fopen(filename, &"r");
    
    while (fgets(line, size_int, f)) {
        if (!isFirst) {
            strcat(text, &newline_str);
        }
        strcat(text, line);
    }
    
    // Free memory from line
    free(line);
    line = NULL;
    
    // Close the file
    fclose(f);
}