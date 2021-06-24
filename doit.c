#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

#define MAX_FILE_STR_SIZE 1024 //1Kb
#define MAX_FILE_SIZE 5242880  //5Mb

char *txtToFnd = NULL;
char *txtToRepls = NULL;
char sBuff[MAX_FILE_STR_SIZE] = {0};
char sBuff2[MAX_FILE_STR_SIZE] = {0};
char *ptrBuff = sBuff;
char *ptrBuff2 = sBuff2;

void findAndRepl(FILE *, FILE *, char *);
void findAndReplEq(FILE *);
char *findAndReplInStr(char *, char *);

int main(int argc, char *argv[])
{
    DIR *ptrDir = NULL;
    struct dirent *dirEnt = NULL;
    FILE *ptrFile = NULL;
    FILE *ptrFileTmp = NULL;
    char *name = NULL;
    char *dir = NULL;
    char fName[_MAX_FNAME] = {0};

    //debug
    //char a1[] = ".\\fd";
    //char a2[] = "z";
    //char a3[] = "xx";
    //dir = a1;
    //txtToFnd = a2;
    //txtToRepls = a3;
    //argc = 4;

    if ((argc != 4)) 
    {
        perror("invalid number of args");
        return 1;
    }

    dir = argv[1];
    txtToFnd = argv[2];
    txtToRepls = argv[3];

    if (strcmp(txtToFnd, txtToRepls) == 0) 
    {
        return 0;
    }

    if (strlen(txtToFnd) > MAX_FILE_STR_SIZE) 
    {
        perror("too large txt_to_find string");
        return 1;
    }

    if (strlen(txtToRepls) > MAX_FILE_STR_SIZE) {
        perror("too large txt_to_repl string");
        return 1;
    }

    if ((strlen(txtToFnd) < MAX_FILE_STR_SIZE) && (strlen(txtToRepls) == MAX_FILE_STR_SIZE)) 
    {
        perror("size of str is overheaded");
        return 1;
    }

    if ((ptrDir = opendir(dir)) == NULL) 
    {
        perror(dir);
        return 1;
    }

    while ((dirEnt = readdir(ptrDir)) != NULL) 
    {
        name = dirEnt->d_name;

        if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
            continue;

        snprintf(fName, strlen(dir) + strlen(name) + 2, "%s\\%s", dir, name);

        if (strlen(txtToFnd) == strlen(txtToRepls))
        {
            if ((ptrFile = fopen(fName, "rb+")) == NULL)
            {
                perror(fName);
                continue;
            }
            findAndReplEq(ptrFile);
            fclose(ptrFile);
        }
        else
        {
            if ((ptrFile = fopen(fName, "r")) == NULL)
            {
                perror(fName);
                continue;
            }
            if ((ptrFileTmp = fopen("tmp", "w+")) == NULL)
            {
                perror("error on tmp creaton");
                return 1;
            }
            findAndRepl(ptrFileTmp, ptrFile, fName);
            fclose(ptrFile);
            fclose(ptrFileTmp);
        }
    }
    closedir(ptrDir);
    return 0;
}

void findAndRepl(FILE *ptrFileTmp, FILE *ptrFile, char *fName)
{
    size_t num;
    size_t file_size;
    char *fbuffer;
    char *fbuffer2;

    while (!feof(ptrFile))
    {
        fseek(ptrFile, 0, SEEK_END);
        file_size = ftell(ptrFile);
        rewind(ptrFile);

        if (file_size >= MAX_FILE_SIZE)
        {
            perror("Too large file");
            exit(1);
        }

        fbuffer = (char *)malloc(MAX_FILE_SIZE);
        fbuffer2 = (char *)malloc(MAX_FILE_SIZE);

        if ((fbuffer == NULL) || (fbuffer2 == NULL))
        {
            perror("Error allocating bytes - less memory.\n");
            exit(1);
        }

        fread(fbuffer, 1, file_size, ptrFile);
        

        //what better
        findAndReplInStr(fbuffer, fbuffer2);
        //what better
        //rewind(ptrFile);
        //if ((fgets(ptrBuff, sizeof(sBuff), ptrFile)) != NULL)
        //{
        //    findAndReplInStr(ptrBuff, ptrBuff2);
        //    strcat(fbuffer, ptrBuff);
        //}

        fwrite(fbuffer, strlen(fbuffer), 1, ptrFileTmp);
        free(fbuffer);
        free(fbuffer2);
    }

    freopen("tmp", "r", ptrFileTmp);
    freopen(fName, "w", ptrFile);

    while ((num = fread(ptrBuff, sizeof(char), MAX_FILE_STR_SIZE, ptrFileTmp)) > 0)
        fwrite(ptrBuff, sizeof(char), num, ptrFile);
}

void findAndReplEq(FILE *ptrFile)
{
    char *strn = NULL;
    long pos;

    while (!feof(ptrFile))
    {
        pos = ftell(ptrFile);
        if ((fgets(ptrBuff, sizeof(sBuff), ptrFile)) != NULL)
        {
            if ((strn = strstr(ptrBuff, txtToFnd)) != NULL)
            {
                fseek(ptrFile, -strlen(strn), 1);
                fwrite(txtToRepls, sizeof(char), strlen(txtToRepls), ptrFile);
                fseek(ptrFile, -(ftell(ptrFile) - pos), 1);
            }
        }
    }
}

char *findAndReplInStr(char *fbuffer, char *fbuffer2)
{
    char *strn = NULL;
    int start = 1;
    int d;
    int i;

    strn = strstr(fbuffer, txtToFnd);
    while (strn != NULL)
    {
        d = strn - fbuffer;
        if ((d == 0) && start)
        {
            strcpy(fbuffer2, txtToRepls);
            strcat(fbuffer2, strn + strlen(txtToFnd));
            start--;
            strcpy(fbuffer, fbuffer2);
            fbuffer2[0] = '\0';
        }
        else if (start)
        {
            i = 0;
            strncpy(fbuffer2, fbuffer, d);
            strcat(fbuffer2, txtToRepls);
            strcat(fbuffer2, strn + strlen(txtToFnd));
            start--;
            strcpy(fbuffer, fbuffer2);
            fbuffer2[0] = '\0';
        }
        else
        {
            strcat(fbuffer2, txtToRepls);
            strcat(fbuffer2, strn + strlen(txtToFnd));
            strcpy(fbuffer + d, fbuffer2);
            fbuffer2[0] = '\0';
        }
        strn = strstr(fbuffer, txtToFnd);
    }
    return fbuffer;
}