#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define S 10000
void createfile (char *address){//alost completely correct
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
                printf("File already exists\n");
            }
            else{
                file = fopen(address,"w");
            }

        }
    }
 void insertstr(char *address, char *string, int line , int index){//almost completely correct except for the ("")part
    /*be carefull you scan the command(insertstr) then you get --file 
    then you are given the address then --str then the string you need to 
    insert then --pos then the line of insertion then ":" and finally the
    index which you should start insertion */

    FILE*file;
    if((file=fopen(address,"r"))==NULL){
        printf("File doesn't exist\n");
        return ;
    }
    
    file = fopen(address,"r");//this is the main file
    FILE*clone_file1,*clone_file2;
    clone_file1 = fopen("root/clone1.txt","w");//this is the cloned file 
    clone_file2 = fopen("root/clone2.txt","w");
    char ch;
    int line_cnt=1;
    int ind=0;
    while((ch=fgetc(file))!=EOF){
        
        if(line_cnt<line){
            fputc(ch,clone_file1);
            if(ch=='\n')
            line_cnt++;
        }
        
        else if(line_cnt==line && ind<index){
            
                fputc(ch,clone_file1);
                ind++;
            
        }
        else {
            fputc(ch,clone_file2);
            if(ch=='\n')
            line_cnt++;
        }
        
    }
    /*now we have two files , the first one contains 
    the part before the position of inserting and the 
    second contains the rest of the main file*/

    fclose(file);
    fclose(clone_file1);
    fclose(clone_file2);
    clone_file1=fopen("root/clone1.txt","a");
    fputs(string,clone_file1);
    fclose(clone_file1);
    clone_file1=fopen("root/clone1.txt","a");
    clone_file2=fopen("root/clone2.txt","r");
    
    while((ch=fgetc(clone_file2))!=EOF){
        fputc(ch,clone_file1);
    }
    fclose(clone_file2);
    fclose(clone_file1);
    clone_file1=fopen("root/clone1.txt","r");
    file=fopen(address,"w");
    while((ch=fgetc(clone_file1))!=EOF){
        fputc(ch,file);
    }
    fclose(file);
    fclose(clone_file1);
    remove("root/clone1.txt");
    remove("root/clone2.txt");

}
void cat(char* address){
    FILE*file=fopen(address,"r");
    char*show=(char*)malloc(S*sizeof(char));
    int n=0;
    char ch;
    while((ch=fgetc(file))!=EOF){
        *(show+n)=ch;
        n++;
    }
    for(int i=0;*(show+i)!='\0';i++){
        printf("%c",*(show+i));
    }
    printf("\n");
    }

int main(){
    createfile("root/file.txt");
    insertstr("root/file.txt","salam khoobi?",1,1);
    cat("root/file.txt");
}