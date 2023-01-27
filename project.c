#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>

#define S 10000
char*content_to_copy;
/*
/struct clipboard{
    char *str ;
    struct clipboard *next;
    struct clipboard *previous;
};
struct clipboard *create(){
    struct clipboard*new1=(struct clipboard*)malloc(sizeof(struct clipboard));
    new1->str=NULL;
    new1->next=NULL;
    new1->previous=NULL;
    return new1;
}
void add_struct(struct clipboard *name,char *string){
    struct clipboard*new1=(struct clipboard*)malloc(sizeof(struct clipboard));
    while(name->next!=NULL){
        name=name->next;
    }
    if(name->str==NULL){
        name->str=string;
    }
    else{
        struct clipboard *new1=create();
        name->next=new1;
        new1->previous=name;
        new1->next=NULL;
        new1->str=string;

    }
}
*/
int check_exist(char*address){
    FILE*file;
    if((file=fopen(address,"r"))==NULL){
        printf("File doesn't exist\n");
        return 0;
    }
    return 1;
}
int find_char(char*string,int line,int index){
    int n=0;
    while(*(string+n)!='\0'){
        n++;
    }
    int line_count=1;
    for(int i=0;i<n;i++){
        if(line==line_count){
            return i+index;
        }
        else if(line>line_count && *(string+i)=='\n'){
            line_count++;
        }
    }

}
char* quotes(char *address1){
    char*address=(char*)malloc(1000*sizeof(char));
    int n=0;
    while(*(address1+n)!='\0'){
        n++;
    }
    if(*address1=='\"'){
        for(int i=0;i<n-2;i++){
            *(address+i)=*(address1+i+1);
        }
        *(address+n-2)='\0';
        return address;
    }
    return address1;
}
   




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
 void insertstr(char *address1, char *string, int line , int index){//almost completely correct except for the ("")part
    /*be carefull you scan the command(insertstr) then you get --file 
    then you are given the address then --str then the string you need to 
    insert then --pos then the line of insertion then ":" and finally the
    index which you should start insertion 
    caution : if the character or the line doesn't exist the program will
    automatically add to the closest place to the given line and index*/
    char* address = quotes(address1);
    FILE*file;
    if(check_exist(address)){
    
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
}
void cat(char* address1){
   char* address = quotes(address1);
    if(check_exist(address)){
    FILE*file=fopen(address,"r");
    char*show=(char*)malloc(S*sizeof(char));
    int n=0;
    char ch;
    while((ch=fgetc(file))!=EOF){
        *(show+n)=ch;
        n++;
    }
    *(show+n)='\0';
    for(int i=0;*(show+i)!='\0';i++){
        printf("%c",*(show+i));
    }
    printf("\n");
    }
}
void removestr(char *address1,int line,int index,int size,char direction){
    char* address = quotes(address1);
    if(check_exist(address)){
    char*content=(char*)malloc(S*sizeof(char));
    FILE*file=fopen(address,"r");
    char ch;
    int n=0;
    while((ch=fgetc(file))!=EOF){
        *(content+n)=ch;
        n++;
    }
    fclose(file);
    // n is the size of content
    int t = find_char(content,line,index);
    if(direction == 'b'){//backwards
        
        for(int i=t;i<n;i++){
            *(content+i-size)=*(content+i);
        }
    }
    
    else if(direction == 'f'){//forwards
        if(t+size<n){
        for(int i=t+size;i<n;i++){
            *(content+i-size)=*(content+i);
        }
    }
    }
    char*content1=(char*)malloc(S*sizeof(char));
    if(direction=='b'||t+size<n){
    for(int i=0;i<n-size;i++){
        *(content1+i)=*(content+i);
    }
    *(content1+n-size)='\0';
    }
    else{
        for(int i=0;i<=t;i++){
            *(content1+i)=*(content+i);
        }
        *(content1+t+1)='\0';
    }
    file=fopen(address,"w");
    fprintf(file,content1);
    fclose(file);
}
}
void copystr(char *address1,int line ,int index,int size,char direction){//almost the same thing as the previous one
/*this function got a bit complicated in addition to what they type in 
you gotta add a struct clipboard *name to your function input
then hopefully the part that's needed will be copied to the clipboard struct
and it's accesible*/
    char* address = quotes(address1);
    if(check_exist(address)){
    char*content=(char*)malloc(S*sizeof(char));
    FILE*file=fopen(address,"r");
    char ch;
    int n=0;
    while((ch=fgetc(file))!=EOF){
        *(content+n)=ch;
        n++;
    }
    fclose(file);
    // n is the size of content
    int k = find_char(content,line,index);
    int t=k-1;
    content_to_copy=(char*)malloc(S*sizeof(char));
    if(direction=='b'){
        for(int i=t-1;i>=t-size;t--){
        *(content_to_copy+size-(t-i))=*(content+i);
        }
        *(content_to_copy+size)='\0';
    }
    else if(direction=='f'){
        for(int i=t+1;i<=t+size;i++){
            *(content_to_copy+i-t-1)=*(content+i);
        }
        *(content_to_copy+size)='\0';
    }
    }
}
void cutstr(char*address1,int line,int index,int size,char direction){
    char* address = quotes(address1);
    if(check_exist(address)){
    copystr(address,line,index,size,direction);
    removestr(address,line,index,size,direction);
    }
}
void pastestr(char* address1,int line , int index){
    char* address = quotes(address1);
    insertstr(address,content_to_copy,line,index);
}

int main(){
    //struct clipboard *init=create();//this is needed for the functions working with clipboard
    //createfile("\"root/dir 1/file 1.txt\"");
    //insertstr("\"root/dir 1/file 1.txt\"","salam\n",2,4);
    //createfile("\"root/dir2/file 1.txt\"");
    //cutstr("\"root/dir 1/file 1.txt\"",1,0,4,'f');
    //pastestr("\"root/dir2/file 1.txt\"",1,0);
    //cat("\"root/dir 1/file 1.txt\"");
    //removestr("\"root/dir 1/file 1.txt\"",1,3,2,'f');
    // cat("root/file.txt");
    // copy("root/file1.txt",1,3,2,'b',init);
    //cutstr("root/dir 1/file 1.txt",1,3,2,'b',init);
    
}