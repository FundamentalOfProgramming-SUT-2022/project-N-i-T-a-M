#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define s 1000
#define t 1000

void createfile (char *dash_file,char *address){
    char a[6]="--file";
    for(int i=0;i<6;i++){
        if(*(dash_file+i)!=a[i]){
            printf("incorrect input");
            return ;
        }
    }
    char b[5]="root/";
    for(int i=0;i<5;i++){
        if(*(address+i)!=b[i]){
            printf("incorrect input");
            return ;
        }
    }
    int cnt=0;
    while(*(address+cnt)!='\0'){
        cnt++;
    }
    for(int i=6;i<cnt;i++){
        if(*(address+i)=='/'){
            int start;
            for(int j=0;j<i;j++){
                if(*(address+j)=='/'){
                    start=j;
                }
            }
            char *name = (char*)malloc((i-start)*sizeof(char));
            for(int j=0;j<i-start-1;j++){
                *(name+j)=*(address+start+j+1);
            }
            DIR *dir =opendir(name);
            if(dir){
                closedir(dir);
            }
            else if(ENOENT == errno){
                char *add=(char*)malloc(i*sizeof(char));
                for(int j=0;j<i;j++){
                    *(add+j)=*(address+j);
                }
                int result = mkdir(add);
            }
        }
        else if(i==cnt){
            if(*(address+i)=='/'){
                printf("incorrect input");
                return ;
            }

            int start;
            for(int j=0;j<i;j++){
                if(*(address+j)=='/'){
                    start=j;
                }
            }
            char *name = (char*)malloc((cnt-start)*sizeof(char));
            for(int j=0;j<cnt-start;j++){
                *(name+j)=*(address+start+j+1);
            }
            FILE *file = fopen(name,"r");
            if(file){
                printf("file already exists");
                fclose(file);
            }
            else{
                fopen(name,"w");
                fclose(file);
            }

        }
    }

    

}
int main(){


}