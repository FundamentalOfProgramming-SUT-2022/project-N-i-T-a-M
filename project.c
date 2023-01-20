#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>


void createfile (char *address){
    int size = 0;
    while(*(address+size)!='\0'){
        size++;
    }
    if(*(address)=='\"'){
        char*address1 =(char*)malloc(100*sizeof(char));
        for(int i=0;i<size-2;i++){
            *(address1+i)=*(address+i+1);
        }
        *(address1+size-2)='\0';
        createfile(address1);
    }
    else {
        for(int i=1;i<size;i++){
            if(*(address+i)=='/'){
                
                char *directory = (char*)malloc(100*sizeof(char));
                for(int j=0;j<i;j++){
                    *(directory+j)=*(address+j);
                }
                *(directory+i)='\0';
                DIR*dir;
                dir=opendir(directory);
                if(dir){
                    closedir(dir);
                }
                else if(ENOENT==errno){
                    int result = mkdir(directory);
                    closedir(dir);
                }
                
            }
            
            }
            FILE*file;
            if((file=fopen(address,"r"))!=NULL){
                fclose(file);
                printf("File already exists");
            }
            else{
                file = fopen(address,"w");
            }

        }
    }

    


int main(){

}