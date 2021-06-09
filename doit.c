#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>

#define SIZE  512

int main(int argc, char* argv[])                  
{   
    DIR * d;
    FILE* f;
    FILE* f1;
    char sbuffer[SIZE];
    char* fn;

    char* fd = argv[1];
    char* t = argv[2];
    char* tr = argv[3];
    unsigned int ts = strlen(t);
    unsigned int trs = strlen(tr);

    if ((d = opendir(fd)) == NULL) {
        perror(fd);
        return 1;
    }
    strcat(fd, "\\");
    struct dirent * dire;
    while ((dire = readdir(d)) != NULL) {
        fn = dire -> d_name;
        char *ffn = malloc(strlen(fd) + strlen(fn) + 1);
        strcpy(ffn, fd);
        strcat(ffn, fn);
        printf ("%s\n", ffn);
        
        if ((f = fopen(ffn, "rt+")) == NULL) {
            perror(ffn);
            continue;
        }
        
        f1 = fopen("tmp", "w+");
        while(!feof (f)) {
            if ((fgets(sbuffer, sizeof(sbuffer), f)) != NULL) {
                char * strn = strstr(sbuffer, t);
                if (strn != NULL) {
                    printf("find %d \n", strn - sbuffer - 1);
                    printf("suffics %s \n", strn + ts);
                    char * prefstr = strtok(sbuffer, t);
                    printf("preffics %s \n", prefstr);
                    //printf("%s\n", strncat("", sbuffer, strn - sbuffer - 1));
                    fprintf(f1, "%s%s%s", prefstr, tr, strn + ts);
                    //fputs(strn, f1);
                    //printf("p - %d", ftell(f));
                    //fseek(f, strlen(sbuffer), 1);
                    //fseek(f, strn - sbuffer - 1, 1);
                    //fputc("V");
                    //fseek(f, strlen(strn), 1);
                } else fprintf(f1, sbuffer);
            }
        }
        size_t num;
        freopen("tmp", "r", f1);
        freopen(ffn, "w", f);
        while((num = fread(sbuffer, sizeof(char), SIZE, f1)) > 0) {
            fwrite(sbuffer, sizeof(char), num, f);
        }
        fclose(f1);
        fclose(f);
    }
    closedir (d);

    
    

    /*while(!feof (f)) {
        if ((fgets(sbuffer, sizeof(sbuffer), f)) != NULL) {
            char * strn = strstr(sbuffer, t);
            if (strn != NULL) {
                printf("%d \n", strn - sbuffer - 1);
                printf("%s \n", strn + ts);
                //printf("p - %d", ftell(f));
                //fseek(f, strlen(sbuffer), 1);
                //fseek(f, strn - sbuffer - 1, 1);
                //fputc("V");
                //fseek(f, strlen(strn), 1);
            }
        }
    }*/

    //fseek(f, n - str - 1, 0);
    //fwrite(tr, sizeof(tr), 1, f);

    //printf("%d \n", ts);
    //printf("%d \n", sizeof(char));
    printf("End! \n");
    //fclose(f);
    return 0;                   
}