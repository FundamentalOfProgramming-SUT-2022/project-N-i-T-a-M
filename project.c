#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include<stdarg.h>
#define S 10000
//remember to handle the problem that if you print sth with non existing file you will get the answer as we
char*content_to_copy;//works as clipboard
//the prototypes 
int find(char*,char*);
void insertstr(char*,char*,int ,int );
void removestr(char *,int ,int ,int ,char );
//function that outputs what you get in input
char*out(char*line,int number){
    int io=0;
    char*output=(char*)calloc(S,sizeof(char));
    //char*output=(char*)calloc(S,sizeof(char));
    int size=0;
    int word_count=0;
    int j=0;
    while(*(line+size)!='\0'){
        //printf("%c",*(line+size));
        size++;
        
    }
    for(int i=0;i<size;i++){
        if(*(line+i)==' '&& io==0){
            word_count++;
            continue;
        }
        
        if(i>=1 && *(line+i)=='"' && *(line+i-1)!='\\'){
            io = !io;
            continue;
        }
        if( word_count==number-1 && i<size-1){
            if(*(line+i)=='\\'){
                i++;
                if(*(line+i)=='\\'){*(output+j)='\\';j++;continue;}
                if(*(line+i)=='"'){*(output+j)='"';j++;continue;}
                if(*(line+i)=='n'){*(output+j)='\n';j++;continue;}
                if(*(line+i)=='t'){*(output+j)='\t';j++;continue;}
            }
            else {
                *(output+j)=*(line+i);
                j++;continue;
            }
        }
        if(word_count==number-1 && i==size-1){
            if(*(line+i)!='"'){
                *(output+j)=*(line+i);
            }
        }
    }
    if(number>word_count){
        return "ERROR";
    }
        return output;
}
int give_line(char*pos){
    char *line=(char*)malloc(10*sizeof(char));
    int i;
    for(i=0;*(pos+i)!=':';i++){
        *(line+i)=*(pos+i);
    }
    int l=0;
    for(int j=0;j<i;j++){
        l*=10;
        l+=*(line+j)-'0';
    }
    return l;   
}
int give_index(char*pos){
    int n=0;
    while(*(pos+n)!=':'){
        n++;
    }
    int index=0;
    for(int i=n+1;*(pos+i)!='\0';i++){
        index*=10;
        index+=*(pos+i)-'0';
    }
    return index;
}
int give_size(char*size){
    int n=0;
    for(int i=0;*(size+i)!='\0';i++){
        n*=10;
        n+=*(size+i)-'0';
    }
    return n;
}
//the side functions
int size(char*string){
    int n=0;
    while(*(string+n)!='\0'){
        n++;
    }
    return n;
}
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
int wildcard(char*string1,char*search1){
    //search is the file content and string is the content we're looking to find
    char*search=quotes(search1);
    char*string=quotes(string1);
    int n=0;
    while(*(search+n)!='\0'){
        n++;
    }    
    int m=0;
    while(*(string+m)!='\0'){
        m++;
    }

    if(*(string+m-1)!='*' && *(string)!='*'){
        for(int i=0;i<=n-m;i++){
            int A=0;
            for(int j=0;j<m;j++){
                if(*(search+i+j)==*(string+j)){
                    A++;
                }
                else if(*(search+i+j)!=*(string+j)){
                    break;
                }
            }
            if(A==m){
                return i;
            }
        }
        return -1;
    }
    else if(*(string+m-1)=='*'){
        char*find=string;
        for(int i=0;i<n;i++){
            int A=0;
            for(int j=0;j<m-1;j++){
                if(*(search+i+j)==*(find+j)){
                    A++;
                }
                else {
                    break;
                }
            }
            if(A==m-1){
                return i;
            }
        }
        return -1;
    }
    else if(*(string)=='*'){
        char*find=string+1;
        for(int i=m-2;i<n;i++){
            int A=0;
            for(int j=0;j<m-1;j++){
                if(*(search+i-j)==*(find+m-2-j)){
                    A++;
                }
                else{
                    break;
                }
            }
            if(A==m-1){
                int ans;
                for(int k=i;k>=0;k--){
                    if(*(search+k)==' '){
                        return  k+1;
                    }
                    else if(k==0){
                        return 0;
                    }
                }
                
            }
        }
        return -1;
    }
}
char* file_to_string(char*address){
    quotes(address);
    char ch;
    char*string=(char*)malloc(S*sizeof(char));
    FILE*file=fopen(address,"r");
    int n=0;
    while((ch=fgetc(file))!=EOF){
        *(string+n)=ch;
        n++;
    }
    return string;
}
int myword(char*string1,int index){//do i count \n or not?
    char*string=quotes(string1);
    if(*(string)!=' '){
        int words=0;
        for(int i=1;i<index;i++){
            if(*(string+i)==' ' && *(string+i-1)!=' '){
                words++;
            }
        }
        return words+1;
    }
    else{
        return myword(string+1,index-1);
    }
    
}
char*line(char*search1,int l){
    char*search=quotes(search1);
    int n=size(search);
    char*myline=(char*)malloc(n*sizeof(char));
    int line_cnt=1;
    int i=1;
    if(l==1){
        *(myline)=*(search);
        while(*(search+i-1)!='\n'){
            *(myline+i)=*(search+i);
            i++;
        }
        return myline;
    }
    int index;
    for(int i=0;i<n;i++){
        if(*(search+i)=='\n'){
            index=i+1;
        }
    }
    myline = line(search+index,l-1);
    return myline;
}
char* shift(char*string,int index,char ch){
    char*str=(char*)calloc(S,sizeof(char));
    for(int i=0;i<index;i++){
        *(str+i)=*(string+i);
    }
    *(str+index)=ch;
    int i;
    for( i=index+1;*(string+i-1)!='\0';i++){
        *(str+i)=*(string+i-1);
    }
    *(str+i)='\0';
    return str;
    
}
int space_number(char*string,int index){
    int s=0;
    if(index>1){
    for(int i=index-1;i>=0;i--){
        if(*(string+i)==' '){
            s++;
        }
        else{
            return s;
        }
    }
    }
    else{
        return 0;
    }
}
void delete_space(char*string,int index){
    int m = size(string);
    if(*(string+index-1)==' '){
        for(int i=index;i<=m;i++){
            *(string+i-1)=*(string+i);
        }
    }
}
//the attributes
/*instead of addresses , i give the file content to the function*/
int count(char*string1,char*search1){
    char*string=quotes(string1);
    char*search=quotes(search1);
    
    if(wildcard(string,search)==-1){
        return 0;
    }
    
    int t = wildcard(string,search);
    return 1+count(string,search+t+1);

}
int at(char *string1,char*search1,int number){
    char*search=quotes(search1);
    char*string=quotes(string1);
    if(count(string,search)<number){
        return -1;
    }
    int t=wildcard(string,search);
    if(number==1){
        return t;
    }
        return t+1+at(string,search+t+1,number-1);
    
            
}
int byword(char*string1,char*search1){
    char*search=quotes(search1);
    char*string=quotes(string1);
    int t = wildcard(string,search);
    if(t==-1){
        return -1;
    }
    return myword(search,t);
}
int* all(char*string1,char*search1){
    char*search=quotes(search1);
    char*string=quotes(string1);
    int cnt=count(string,search);
    int *ans=(int*)malloc(cnt*sizeof(int));
    for(int i=0;i<cnt;i++){
        *(ans+i)=at(string,search,i+1);
    }
    return ans;
}
char* replace_at(char*string11,char*string21,char*search1,int index){
    char*string1=quotes(string11);
    char*string2=quotes(string21);
    char*search=quotes(search1);
    int t=at(string1,search,index);
    int n=size(string1);
    int m=size(string2);
    int k=size(search);
    char*search_copy=(char*)malloc((k+m-n+1)*sizeof(char));
    for(int i=k;i>=t+n;i--){
        *(search_copy+m-n+i)=*(search+i);
    }
    for(int i=t;i<t+m;i++){
        *(search_copy+i)=*(string2+i-t);
    }
    for(int i=0;i<t;i++){
        *(search_copy+i)=*(search+i);
    }
    return search_copy;
}
char*replace_all(char*string11,char*string21,char*search1){
    char*search=quotes(search1);
    char*string1=quotes(string11);
    char*string2=quotes(string21);
    int n=count(string1,search);
    int t=size(string2);
    int k=size(string1);
    //char*search_copy=(char*)malloc((k-t)*n*sizeof(int));
    if(n==0){
        printf("string not found!\nPlease try something else.\n");
    }
    else if(n==1){
        printf("You wanted it and you got it :)\n");
        return replace_at(string1,string2,search,1);
        
    }
    else{
        while(n){
            search=replace_at(string1,string2,search,n);
            n--;
        }
        printf("You wanted it and you got it :)\n");
        return search;
    }

}
void removestr_without_error(char *address1,int line,int index,int size,char direction){
    char* address2 = quotes(address1);
    char*address = address2+1;
   
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
void copystr_without_error(char *address1,int line ,int index,int size,char direction){//almost the same thing as the previous one
/*this function got a bit complicated in addition to what they type in 
you gotta add a struct clipboard *name to your function input
then hopefully the part that's needed will be copied to the clipboard struct
and it's accesible*/
    char* address2 = quotes(address1);
    char*address = address2+1;
    
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
void insertstr_without_error(char *address1, char *string, int line , int index){//almost completely correct except for the ("")part
    char* address2 = quotes(address1);
    char*address = address2+1;
    FILE*file;
    
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
//the main functions
void createfile (char *address1){//alost completely correct
    char*address2=quotes(address1);
    char*address=address2+1;
    int size = 0;
    while(*(address+size)!='\0'){
        size++;
    }
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
                printf("File already exists\n");
                fclose(file);
            }
            else{
                file = fopen(address,"w");
            }    
    }
void insertstr(char *address1, char *string, int line , int index){//almost completely correct except for the ("")part
    char* address2 = quotes(address1);
    char*address = address2+1;
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
    char* address2 = quotes(address1);
    char*address = address2+1;
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
    char* address2 = quotes(address1);
    char*address = address2+1;
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
    char* address2 = quotes(address1);
    char*address = address2+1;
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
    copystr_without_error(address,line,index,size,direction);
    removestr_without_error(address,line,index,size,direction);
    }
}
void pastestr(char* address1,int line , int index){
    char* address = quotes(address1);
    if(check_exist(address))
    insertstr_without_error(address,content_to_copy,line,index);
    
}
int find(char*string1,char*address1){//should i print the answer here or not
    char* address2 = quotes(address1);
    char*address = address2+1;
    char*string=quotes(string1);
    char*search=file_to_string(address);
    if(check_exist(address)){

    }
}
void replace(char*string11,char*string21,char*address){

}
void grep(char*string1,char*search1){
    char*search = quotes(search1);
    char*string = quotes(string1);
    int n = size(search);
    int line_cnt = 1;
    for(int i=0;i<n;i++){
        if(*(search+i)=='\n'){
            line_cnt++;
        }
    }
    for(int i=1;i<=line_cnt;i++){
        char*myline=line(search,i);
        int t=0;
        while(*(myline+t)!='\n'){
            t++;
        }
        *(myline+t+1)='\0';
        if(wildcard(string,myline)!=-1){
            for(int j=0;j<=t;j++){
                printf("%c",*(myline+j));
            }
        }
    }

}
void auto_indent(char*address1){
    char*address=address1+1;
    if(check_exist(address)){
        char*field=file_to_string(address);
        int n = size(field);
        int cnt=0;
        
}
}
void tree(char *basePath, const int root, int depth){
    int i;
    char *path=(char*)malloc(1000*sizeof(char));
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    if (!dir){
        return;
    }
    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            for (i=0; i<root; i++){
                if (i%2 == 0 || i == 0){
                    printf("%c", 179);
                }
                else{
                    printf(" ");
                }
            }
            printf("%c%c%s\n", 195, 196, dp->d_name);
            if((depth==-1)||(depth>=root/2+1)){
                strcpy(path, basePath);
                strcat(path, "/");
                strcat(path, dp->d_name);
                tree(path, root + 2, depth);
            }
        }
    }
    closedir(dir);
}
/*int main(){
    char*str=(char*)malloc(S*sizeof(char));
    char*command=(char*)malloc(100*sizeof(char));
    //char*address=(char*)malloc(1000*sizeof(char));
    //char*string=(char*)malloc(1000*sizeof(char));
    scanf("%[^\n]%*c", str);
    sscanf(str,"%s",command);
    
    if(strcmp(command,"createfile")==0){
        //do create file
        char*address=out(str,3);
        createfile(address);
        
    }
    if(strcmp(command,"insertstr")==0){
        //do insertstr
        char*address=out(str,3);
        char*string=out(str,5);
        char*pos=out(str,7);
        int line=give_line(pos);
        int index=give_index(pos);
        insertstr(address,string,line,index);
        
    }
    if(strcmp(command,"cat")==0){
        //do cat
        char*address=out(str,3);
        cat(address);
    }
    if(strcmp(command,"removestr")==0){
        //do removestr
        char*address=out(str,3);
        char *pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        char*cize=out(str,7);
        int size=give_size(cize);
        char*direction=out(str,8);
        char direc=*(direction+1);
        removestr(address,line,index,size,direc);

    }
    if(strcmp(command,"copystr")==0){
        //do copystr
        char*address=out(str,3);
        char *pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        char*cize=out(str,7);
        int size=give_size(cize);
        char*direction=out(str,8);
        char direc=*(direction+1);
        copystr(address,line,index,size,direc);
    }
    if(strcmp(command,"cutstr")==0){
        //do cutstr
        char*address=out(str,3);
        char *pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        char*cize=out(str,7);
        int size=give_size(cize);
        char*direction=out(str,8);
        char direc=*(direction+1);
        cutstr(address,line,index,size,direc);
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
        char*address=out(str,3);
        char*pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        pastestr(address,line,index);
    }
    if(strcmp(command,"find")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
    if(strcmp(command,"pastestr")==0){
        //do pastestr
    }
}

*/











int main(){
    //struct clipboard *init=create();//this is needed for the functions working with clipboard
    //createfile("root/file.txt");
    //insertstr("root/file.txt","h h h",1,0);
    //createfile("\"root/dir2/file 1.txt\"");
    //cutstr("\"root/dir 1/file 1.txt\"",1,0,4,'f');
    //pastestr("\"root/dir2/file 1.txt\"",1,0);
    //cat("\"root/dir 1/file 1.txt\"");
    //removestr("\"root/dir 1/file 1.txt\"",1,3,2,'f');
    // cat("root/file.txt");
    // copy("root/file1.txt",1,3,2,'b',init);
    //cutstr("root/dir 1/file 1.txt",1,3,2,'b',init);
    //int t = find("a","root/file.txt");
    //printf("%d",t);
    // int t= wildcard("h","hhhi how are you?");
    // printf("%d\n",t);
    // int t = count("smart pants","I thought i was a smart pants then i relized heh no way");
    // printf("%d\n",t);
    //int s = at("hi","hi hhhhh",1);
    //printf("%d",s);
    //int t1 = byword("t","hello there baby!");
    //printf("%d",t1);
    //int *ans = all("h","hhhi how are you?");
    //printf("%d",*(ans+3));
    //printf("%d",at("hi","hi how are you hi how  doing?",1));
    // char*str="hi how are you hi how  doing?";
    // char*ans=replace_all("h*","hip",str);
    // for(int i=0;i<(count("hi",str)*1)+size(str);i++){
    //     printf("%c",*(ans+i));
    // }
    //grep("str","str\nstring\nhi\nhi i am string\nbye");
    
    //auto_indent("/root/file.txt");
    //cat("/root/file.txt");
    
}




