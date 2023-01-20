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
        
    }
    else {
        for(int i=1;i<size;i++){
            if(*(address+i)=='\\'){
                
                char *directory = (char*)malloc(i*sizeof(char));
                for(int j=0;j<i;j++){
                    *(directory+j)=*(address+j);
                }
                
                DIR*dir;
                dir=opendir(directory);
                if(dir){
                    closedir(dir);
                }
                else if(ENOENT==errno){
                    int result = mkdir(directory);
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
char *a={"root\\dir1\\file.txt"};
for(int i=0;i<19;i++){
    printf("%c\t",*(a+i));
}
createfile(a);
}