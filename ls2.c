#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ls2.h"

//no pattern to match, simply print directory
void lsMode1(char* directory, int level){
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);
    if (dir == NULL){
        return;
    }
    while((entry = readdir(dir))!= NULL){
        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0){
            char *path = malloc(strlen(directory) + strlen(entry->d_name) + 2);
            sprintf(path, "%s/%s", directory, entry->d_name);
            struct stat stats;
            if(stat(path, &stats) == 0){
                if(S_ISREG(stats.st_mode)){
                    for(int i =0; i<level; i++){
                        printf("%s", INDENT);
                    }
                    printf("%s (%ld bytes)\n", entry->d_name, stats.st_size);
                }
                else if(S_ISDIR(stats.st_mode)){
                    for(int i =0; i<level; i++){
                        printf("%s", INDENT);
                    }
                    printf("%s/ (directory)\n", entry->d_name);
                    lsMode1(path, level+1);
                }
            }
            free(path);
        }
    }
    closedir(dir);
    
}

//pattern to match, must search directory for pattern and use the stack to store output
int lsMode2(char* directory, char* pattern, int level, stack_t* stack){
    int found = FALSE;
    DIR *dir;
    struct dirent *entry;
    dir = opendir(directory);
    if (dir == NULL){
        return FALSE;
    }
    while((entry = readdir(dir))!= NULL){
        if (strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, ".") != 0){
            char *path = malloc(strlen(directory) + strlen(entry->d_name) + 2);
            sprintf(path, "%s/%s", directory, entry->d_name);
            struct stat stats;
            if(stat(path, &stats) == 0){
                if(S_ISREG(stats.st_mode)){
                    if(strcmp(entry->d_name, pattern) == 0){
                        char *toPrint = malloc(100);
                        char *indent = malloc(level*(strlen(INDENT)));
                        indent[0] = '\0';
                        found = TRUE;
                        for(int i =0; i<level; i++){
                            strcat(indent, INDENT);
                        }
                        sprintf(toPrint, "%s%s (%ld bytes)", indent, entry->d_name, stats.st_size);
                        push(stack, toPrint);
                        free(indent);
                        free(toPrint);
                    }
                }
                else if(S_ISDIR(stats.st_mode)){
                    int foundInside = lsMode2(path, pattern, level+1, stack);
                    if (foundInside == TRUE){
                        found = TRUE;
                        char *indent = malloc(level*(strlen(INDENT)));
                        indent[0] = '\0';
                        for(int i =0; i<level; i++){
                            strcat(indent, INDENT);
                        }
                        char *toPrint = malloc(100);
                        sprintf(toPrint, "%s%s/ (directory)", indent, entry->d_name);
                        push(stack, toPrint);
                        free(indent);
                        free(toPrint);
                    }
                }
            }
            free(path);
        }
    }
    closedir(dir);
    return found;
}