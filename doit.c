#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

#define MAX_FILE_STR_SIZE 1024

char* txtToFnd = NULL;
char* txtToRepls = NULL;
char sBuff[MAX_FILE_STR_SIZE];
char* ptrBuff = sBuff;

void findandrepl(FILE*, FILE*, char*);
void findandrepleq(FILE*);

int main(int argc, char* argv[]) {
    DIR* ptrDir = NULL;
    struct dirent* dirEnt = NULL;
    FILE* ptrFile = NULL;
    FILE* ptrFileTmp = NULL;
    char fName[MAX_FILE_STR_SIZE];//char* ffName = malloc(strlen(dir) + strlen(fName) + 2);
    unsigned short fType;
    char* dir = NULL;

    //debug
    char a1[] = ".\\fd";
    char a2[] = "zz";
    char a3[] = "x";
    dir = a1;
    txtToFnd = a2;
    txtToRepls = a3;
    argc = 4;

    if ((argc != 4)) {
        perror("invalid number of args"); 
        return 0;
    }
    
    if (strcmp(txtToFnd, txtToRepls) == 0) {
        return 0;
    }

    //dir = argv[1];
    //txtToFnd = argv[2];
    //txtToRepls = argv[3];

    if ((ptrDir = opendir(dir)) == NULL) {
        perror(dir);
        return 1;
    }
  
    while ((dirEnt = readdir(ptrDir)) != NULL) {
        char* name = dirEnt -> d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) 
            continue;
        
        snprintf(fName, sizeof(fName), "%s\\%s", dir, name);
        
        if (strlen(txtToFnd) == strlen(txtToRepls)) {
            if ((ptrFile = fopen(fName, "rb+")) == NULL) {
                perror(fName); 
                continue;
            }
            findandrepleq(ptrFile);
            fclose(ptrFile);
        } else {
            if ((ptrFile = fopen(fName, "r")) == NULL) {
                perror(fName); 
                continue;
            }
            if ((ptrFileTmp = fopen("tmp", "w+")) == NULL) {
                perror("error on tmp creaton");
                return 1;
            }
            findandrepl(ptrFileTmp, ptrFile, fName);
            fclose(ptrFile);
            fclose(ptrFileTmp);
        }
    }
    closedir (ptrDir);
    return 0;                   
}


void findandrepl(FILE* ptrFileTmp, FILE* ptrFile, char* fName) {
    char * strn = NULL;
    size_t num;
    char sBuff2[MAX_FILE_STR_SIZE];
    char* ptrBuff2 = sBuff2;
    while(!feof (ptrFile)) {
        if ((fgets(ptrBuff, sizeof(sBuff), ptrFile)) != NULL) {
            strcpy(sBuff2, "");
            char * preffix = strtok(ptrBuff, txtToFnd);
            while (preffix != NULL) {
                if (strstr(ptrBuff, txtToFnd) != NULL) {
                    strcat(sBuff2, txtToRepls);
                    strcpy(ptrBuff, "");
                }
                strcat(sBuff2, preffix);
                char t = preffix[strlen(preffix)-1];
                if ((strcmp(preffix,"\n") != 0) && (t != '\n'))
                    strcat(sBuff2, txtToRepls);
                preffix = strtok (NULL, txtToFnd);
            }
            fprintf(ptrFileTmp, "%s", ptrBuff2);
            //if ((strn = strstr(ptrBuff, txtToFnd)) != NULL) {
            //    //int d = strn - ptrBuff - 1;
            //    char* suffix = strn + strlen(txtToFnd);
            //    fprintf(ptrFileTmp, "%s%s%s", preffix, txtToRepls, suffix);
            //} else fprintf(ptrFileTmp, ptrBuff);
        }
    }

    freopen("tmp", "r", ptrFileTmp);
    //freopen(fName, "w", ptrFile);

    //while((num = fread(ptrBuff, sizeof(char), MAX_FILE_STR_SIZE, ptrFileTmp)) > 0) {
    //    fwrite(ptrBuff, sizeof(char), num, ptrFile);
    //}
}

void findandrepleq(FILE* ptrFile) {
    char * strn = NULL;

    while(!feof (ptrFile)) {
        long p = ftell(ptrFile);
        long pos = ftell(ptrFile);
        if ((fgets(ptrBuff, sizeof(sBuff), ptrFile)) != NULL) {
            p = ftell(ptrFile);
            if ((strn = strstr(ptrBuff, txtToFnd)) != NULL) {
                fseek(ptrFile, - strlen(strn), 1);
                p = ftell(ptrFile);
                fwrite(txtToRepls, sizeof(char), strlen(txtToRepls), ptrFile);
                p = ftell(ptrFile);
                fseek(ptrFile, -(ftell(ptrFile) - pos), 1);
                p = ftell(ptrFile);
                p = ftell(ptrFile);
            }
        }
    }
}