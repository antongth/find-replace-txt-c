#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

#define SIZE 1024

char* txtToFnd = NULL;
char* txtToRepls = NULL;
char sBuff[SIZE];

void findandrepl(FILE*, FILE*, char*);
void findandrepleq(FILE* ptrFile);

int main(int argc, char* argv[]) {

    DIR* ptrDir = NULL;
    struct dirent* dirEnt = NULL;
    FILE* ptrFile = NULL;
    FILE* ptrFileTmp = NULL;
    char fName[SIZE];//char* ffName = malloc(strlen(dir) + strlen(fName) + 2);
    unsigned short fType;
    char* dir = NULL;

    //debug
    char a1[] = ".\\fd";
    char a2[] = "zz";
    char a3[] = "xx";
    dir = a1;
    txtToFnd = a2;
    txtToRepls = a3;

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

    //strcat(dir, "\\");
  
    while ((dirEnt = readdir(ptrDir)) != NULL) {

        if ((fType = dirEnt -> d_type) == 16) 
            continue;
        
        snprintf(fName, sizeof(fName), "%s\\%s", dir, dirEnt -> d_name);
        //strcpy(fName, dir);
        //strcat(fName, dirEnt -> d_name);
        
        if (strlen(txtToFnd) == strlen(txtToRepls)) {
            if ((ptrFile = fopen(fName, "r+")) == NULL) {
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

    char * strn;
    size_t num;
    
    while(!feof (ptrFile)) {
        if ((fgets(sBuff, sizeof(sBuff), ptrFile)) != NULL) {
            if ((strn = strstr(sBuff, txtToFnd)) != NULL) {
                printf("find %d \n", strn - sBuff - 1);
                printf("suffics %s \n", strn + strlen(txtToFnd));
                char * prefstr = strtok(sBuff, txtToFnd);
                printf("preffics %s \n", prefstr);
                //printf("%s\n", strncat("", sBuff, strn - sBuff - 1));
                fprintf(ptrFileTmp, "%s%s%s", prefstr, txtToRepls, strn + strlen(txtToFnd));
                //fputs(strn, ptrFileTmp);
                //printf("p - %d", ftell(f));
                //fseek(f, strlen(sBuff), 1);
                //fseek(f, strn - sBuff - 1, 1);
                //fputc("V");
                //fseek(f, strlen(strn), 1);
            } else fprintf(ptrFileTmp, sBuff);
        }
    }

    freopen("tmp", "r", ptrFileTmp);
    freopen(fName, "w", ptrFile);

    while((num = fread(sBuff, sizeof(char), SIZE, ptrFileTmp)) > 0) {
        fwrite(sBuff, sizeof(char), num, ptrFile);
    }
}

void findandrepleq(FILE* ptrFile) {

    char * strn;

    while(!feof (ptrFile)) {
        if ((fgets(sBuff, sizeof(sBuff), ptrFile)) != NULL) {
            strn = strstr(sBuff, txtToFnd);
            if (strn != NULL) {
                fseek(ptrFile, strlen(sBuff), 1);
                fseek(ptrFile, strn - sBuff - 1, 1);
                fputs(txtToRepls, ptrFile);
                fseek(ptrFile, strlen(strn), 1);
            }
        }
    }
    //fseek(ptrFile, n - txtToRepls - 1, 0);
    //fwrite(txtToRepls, sizeof(txtToRepls), 1, ptrFile);
}