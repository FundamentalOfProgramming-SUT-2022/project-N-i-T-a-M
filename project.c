#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include<stdarg.h>
#define S 10000
int GREP_C=0;

//remember to handle the problem that if you print sth with non existing file you will get the answer as we
char content_to_copy[10000];//works as clipboard
//the prototypes 
int SIZE;
void createfile(char*);
void find(char*,char*,int,char*);
void insertstr(char*,char*,int ,int );
void removestr(char *,int ,int ,int ,char );
int F(char*,char*);
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
                //if(*(line+i)=='*'){*(output+j)='*';j++;continue;}
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
    if(number-1>word_count){
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
int check_position(int line,int index,char*string){
    int cnt=1;
    for(int i=0;*(string+i)!='\0';i++){
        if(*(string+i)=='\n'){
            cnt++;
        }
        if(line==cnt){
            int s=0;
            for(int j=i+1;(*(string+j)!='\n'&&*(string+j)!='\0'&&*(string+i)!=EOF);j++){
                s++;
            }
            if(index<0||index>s){
                printf("Index out of bounds!");
                return 0;
            }
        }
    }
    if(line==0||line>cnt){
        printf("Index out of bounds!");
        return 0;
    }
    return 1;

}
int min(int a,int b){
    if(a>b)return b;
    else return a;
}
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
        if(*(string+i)=='\n'){
            line_count++;
        }
        if(line==line_count){
            return i+index;
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
void delete_char(char*string,int index){
    int n=strlen(string);
    for(int i=index;i<n;i++){
        *(string+i)=*(string+i+1);
    }
    *(string+n-1)='\0';
}
void createfile_without_output (char *address1){//alost completely correct
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
                //printf("File already exists\n");
                fclose(file);
            }
            else{
                file = fopen(address,"w");
            }    
    }
void delete_space(char*string){
    int n = strlen(string);
    for(int i=0;i<n;i++){
        if(*(string+i)==' ' && *(string+i+1)=='{'){
            delete_char(string,i);
            i--;
        }
        if(*(string+i)==' ' && *(string+i-1)=='}'){
            delete_char(string,i);
            i--;
        }
    }
}
void add_to_memorry(char*address1){
    char*address=address1+1;
    char*new_address=address+1;
    //if(check_exist(address)){
    createfile_without_output(address);
    FILE*new_file=fopen(new_address,"w");
    FILE*file=fopen(address,"r");
    char ch;
    while((ch=fgetc(file))!=EOF){
        fputc(ch,new_file);
    }
    fclose(file);
    fclose(new_file);
    //}
}
void cycle(char*address1){
    if(check_exist(address1+1)){
    FILE*memmory=fopen(address1+2,"r");
    FILE*tmp=fopen("file_to_hold_memmory.txt","w");
    char ch;
    while((ch=fgetc(memmory))!=EOF){
        fputc(ch,tmp);
    }
    fclose(tmp);fclose(memmory);
    FILE*file=fopen(address1+1,"r");
    memmory=fopen(address1+2,"w");
    while((ch=fgetc(file))!=EOF){
        fputc(ch,memmory);
    }
    fclose(file);fclose(memmory);
    file=fopen(address1+1,"w");
    tmp=fopen64("file_to_hold_memmory.txt","r");
    while((ch=fgetc(tmp))!=EOF){
        fputc(ch,file);
    }
    fclose(file);fclose(tmp);
    remove("file_to_hold_memmory.txt");
    }
}
//the attributes
//instead of addresses , i give the file content to the function
int count(char*string,char*search){
    int cnt=0;
    for(int i=0;*(search+i)!='\0';i++){
        if(F(string,search+i)) cnt++;
    }
    return cnt;

}
int at(char *string,char*search,int number){
    if(count(string,search) < number){
        return -1;
    }
    int cnt=0;
    int n = strlen(search);
    for(int i=0;i<n;i++){
        if(F(string,search+i)) cnt++;
        if(cnt==number)return i;
    }  
    return -1;
}
int byword(char*string,char*search){
    int t = at(string,search,1);
    return myword(search,t);
}
int* all(char*string,char*search){
    int cnt=count(string,search);
    int *ans=(int*)malloc(cnt*sizeof(int));
    if(cnt==0){
        printf("There's no such string in this file .");
        return NULL;
    }
    for(int i=0;i<cnt;i++){
        *(ans+i)=at(string,search,i+1);
    }
    return ans;
}
char*replace_at(char*string1,char*string2,char*search,int index){
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
    //*(search_copy+k+1+m-n)='\0';
    return search_copy;
}
char*replace_all(char*string1,char*string2,char*search){
    int n=count(string1,search);
    int t=size(string2);
    int k=size(string1);
    char*search1=(char*)malloc(S*sizeof(char));
    //char*search_copy=(char*)malloc((k-t)*n*sizeof(int));
    if(n==0){
        printf("string not found!\nPlease try something else.\n");
    }
    else if(n==1){
        printf("SUCCESS\n");
        return replace_at(string1,string2,search,1);
        
    }
    else{
        while(n){
            search1 = replace_at(string1,string2,search,n);
            n--;
        }
        printf("SUCCESS\n");
        return search1;
    }

}
int F(char*small,char*big){
    int m=strlen(small);
    int n=strlen(big);
    if(m>n) return 0;
    for(int i=0;i<m;i++){
        if(*(small+i)=='\\' && *(small+i+1)=='*' && i<m-2){
            if(*(big+i)!='*') return 0;
            return F(small+i+2,big+i+1);
        }
        else if(i=m-2 && *(small+i)=='\\' && *(small+i+1)=='*'){
            if(*(big+i)!='*') return 0;
            else return 1;
        }
        else if(*(small+i)!='*'&&i<m-1){
            if(*(big+i)!=*(small+i)) return 0;
            else return F(small+1,big+1);
        }
        else if(i==m-1&&*(small+i)!='*'){
            if(*(big+i)!=*(small+i)) return 0;
            else return 1;   
        }
        else if(*(small+i)=='*'){
            for(int j=i+1;(*(big+j-1)!=' ' && *(big+j-1)!='\n' && *(big+j-1)!='\0' && *(big+j-1)!=EOF && j <= n );j++){
                if(F(small+i+1,big+j)) return 1;
            }
            return 0;
        }
    } 
    return 1;   
}
void removestr_without_error(char *address1,int line,int index,int size,char direction){
    char*address = address1+1;
    char*field=file_to_string(address);
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
   char*address = address1+1;
    char*content=file_to_string(address);
    int k = find_char(content,line,index);
    int t=k-1;
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
    FILE*file=fopen("clipboard/clipboard.txt","w");
    fputs(content_to_copy,file);
    fclose(file);
    }
void insertstr_without_error(char *address1, char *string, int line , int index){//almost completely correct except for the ("")part
    char* address2 = quotes(address1);
    char*address = address2+1;
    FILE*file;
    char*field=file_to_string(address);
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
void createfile (char *address1){//checked
    
    char*address=address1+1;
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
void insertstr(char *address1, char *string, int line , int index){//checked
    add_to_memorry(address1);
    char* address2 = quotes(address1);
    char*address = address2+1;
    FILE*file;
    if(check_exist(address)){
    char*field=file_to_string(address);
    //if(check_position(line,index,field)){
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
void cat(char* address1){//checked
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
void removestr(char *address1,int line,int index,int size,char direction){//checked
    add_to_memorry(address1);
    char*address = address1+1;
    if(check_exist(address)){
    char*field=file_to_string(address);
    //if(check_position(line,index,field)){
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
void copystr(char *address1,int line ,int index,int size,char direction){//checked
    char*address = address1+1;
    if(check_exist(address)){
    char*content=file_to_string(address);
    int k = find_char(content,line,index);
    int t=k-1;
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
    FILE*file=fopen("clipboard/clipboard.txt","w");
    fputs(content_to_copy,file);
    fclose(file);
    }
    }
void cutstr(char*address1,int line,int index,int size,char direction){//checked
    add_to_memorry(address1);
    char* address = address1+1;
    if(check_exist(address)){
    char*str=file_to_string(address);
    copystr_without_error(address1,line,index,size,direction);
    removestr_without_error(address1,line,index,size,direction);
    }
    }
void pastestr(char* address1,int line , int index){
    add_to_memorry(address1);
    char* address= address1+1;
    if(check_exist(address)){
    char*content=file_to_string("clipboard/clipboard.txt");
    insertstr_without_error(address1,content,line,index);
    }
}
void find(char*string,char*address1,int format,char*seven){//should i print the answer here or not
    char*address = address1+1;
    if(check_exist(address)){
        char*search=file_to_string(address);
        if(format==1){//at 
            if(strcmp(seven,"ERROR")==0){
                int t=at(string,search,1); 
                printf("%d\n",t);
            }
            else {
                int n=0;
                for(int i=0;*(seven+i)!='\0';i++){
                    n*=10;
                    n+=*(seven+i)-'0';
                }
                int t=at(string,search,n);
                printf("%d\n",t);
            }
        }
        else if(format==2){//count
            int t=count(string,search);
            printf("%d\n",t);
        }
        else if(format==3){//byword
            int t=byword(string,search);
            printf("%d\n",t);
        }
        else if(format==4){//all 
            int *t=all(string,search);
            int n=count(string,search);
            for(int i=0;i<n-1;i++){
                printf("%d,",*(t+i));
            }
            printf("%d\n",*(t+n-1));
        }
        else if(format==5){//at byword
            int a=0;
            for(int i=0;*(seven+i)!='\0';i++){
                a*=10;
                a+=*(seven+i)-'0';
            }
            if(count(string,search)){
                int*words=(int*)malloc(100*sizeof(int));
                int cnt=0;
                int *indexes=all(string,search);
                int n=count(string,search);
                for(int i=0;i<n;i++){
                    if(myword(search,*(indexes+i))!=myword(search,*(indexes+i+1))){
                        int t=myword(search,*(indexes+i));
                        //printf("%d,",t);
                        *(words+cnt)=t;
                        cnt++;
                    }
                }
                //printf("%d",myword(search,*(indexes+n-1)));
                *(words+cnt)=myword(search,*(indexes+n-1));
                if(a<=cnt)
                printf("%d\n",*(words+a));
                else 
                printf("There's not %d of it in the file\n",a);
            }
            else
            printf("There's not any of this word in the file !\n");
        }
        else if(format==6){//count byword
            if(count(string,search)){
                int *indexes=all(string,search);
                int n=count(string,search);
                int cnt=1;
                for(int i=0;i<n;i++){
                    if(myword(search,*(indexes+i))!=myword(search,*(indexes+i+1))){
                        cnt++;
                    }
                }
                printf("%d\n",cnt);
            }
            else
            printf("%d\n",0);
        }
        else if(format==7){//all byword
            if(count(string,search)){
                int *indexes=all(string,search);
                int n=count(string,search);
                for(int i=0;i<n;i++){
                    if(myword(search,*(indexes+i))!=myword(search,*(indexes+i+1))){
                        int t=myword(search,*(indexes+i));
                        printf("%d,",t);
                    }
                }
                printf("%d\n",myword(search,*(indexes+n-1)));
            }
        }
            //printf("jds");
    }
}
void replace(char*string1,char*string2,char*address1,int mode,char*number){
    add_to_memorry(address1);
    char*address=address1+1;
    if(check_exist(address)){
        char*search=file_to_string(address);
        if(mode==1){//at sth
        int n=0;
        if(strcmp(number,"ERROR")==0){n=1;}
        else{
            for(int i=0;*(number+i)!='\0';i++){
                n*=10;
                n+=*(number+i)-'0';
            }
        }
            if(count(string1,search)==0){
                printf("There's no such string in the file.\n");
            }
            else if((count(string1,search) ) < n){
                printf("There's not %d of this string\n",n);
            }
            else{
                char* changed = replace_at(string1,string2,search,n);
                FILE*file=fopen(address,"w");
                fputs(changed,file);
                fclose(file);
                printf("SUCCESS\n");
            }

        }
        if(mode==2){//all
        //int n=count(string1,search);
            char*changed = replace_all(string1,string2,search);
                FILE*file=fopen(address,"w");
                fputs(changed,file);
                fclose(file);
        }
    }
}
void grep(char*string,char*address1,int mode){
    char*address=address1+1;
    if(check_exist(address)){
    if(mode==1){
        FILE*file=fopen(address,"r");
        char*line=(char*)malloc(1000*sizeof(char));
        char*search=file_to_string(address);
        int l=1;
        for(int i=0;*(search+i)!='\0';i++){
            if(*(search+i)=='\n')
            l++;
        }
        while(l--){
            fgets(line,1000,file);
            if(line==NULL)
                break;
            else if(count(string,line)){
                printf("%s",line);
                int n=strlen(line);
                if(*(line+n-1)!='\n')
                printf("\n");
                
            }
        }
        fclose(file);
    }
    else if(mode==2){
        FILE*file=fopen(address,"r");
        char*line=(char*)malloc(1000*sizeof(char));
        char*search=file_to_string(address);
        int l=1;
        for(int i=0;*(search+i)!='\0';i++){
            if(*(search+i)=='\n')
            l++;
        }
        while(l--){
            fgets(line,1000,file);
            if(line==NULL)
                break;
            else if(count(string,line)){
                //printf("%s",line);
                //int n=strlen(line);
                //if(*(line+n-1)!='\n')
                //printf("\n");
                GREP_C++;
            }
        }
        fclose(file);
    }
    else if(mode==3){
        FILE*file=fopen(address,"r");
        char*search=file_to_string(address);
        if(count(string,search)){
            printf("%s",address1);
            printf("\t");
        }
    }
}
}
void undo(char*address1){
    char*address=address1+1;
    if(check_exist(address)){
        cycle(address1);
    }
}
void auto_indent(char*address1){
    add_to_memorry(address1);
    char*address=address1+1;
    char*content=(char*)malloc(S*sizeof(char));
    int ind=0;
    if(check_exist(address)){
        char*field=file_to_string(address);
        int n=strlen(field);
        int cnt=0;
        delete_space(field);
        for(int i=0;i<n;i++){
            if( *(field+i)!='{' && *(field+i)!='}'){
                //printf("%c",*(field+i));
                *(content+ind)=*(field+i);
                ind++;
                
            }
            else if(*(field+i)=='{'){
                if(i==0||*(field+i-1)=='\t'||*(field+i-1)=='\n'||*(field+i-1)=='}'||*(field+i-1)=='{'){
                    cnt++;
                    //printf("{\n");
                    *(content+ind)='{';
                    ind++;
                    if(*(field+i+1)!='\n'){
                    *(content+ind)='\n';
                    ind++;
                    }
                    int x=cnt;
                    while(x>0){
                        //printf("\t");
                        *(content+ind)='\t';
                        ind++;
                        x--;
                    }  
                }
                else{
                    //printf(" {\n");
                    *(content+ind)=' ';
                        ind++;
                    *(content+ind)='{';
                        ind++;
                    if(*(field+i+1)!='\n'){
                        *(content+ind)='\n';
                        ind++;
                    }
                    cnt++;
                     
                    int x=cnt;
                    while(x>0){
                        //printf("\t");
                        *(content+ind)='\t';
                        ind++;
                        x--;
                    }
                }
            }
            else if(*(field+i)=='}'){
                if(*(field+i-1)!='\t'||*(field+i-1)!='\n'){
                    if(*(field+i+1)!='\n'){
                        *(content+ind)='\n';
                        ind++;
                    }
                    cnt--;
                    int x=cnt;
                    while(x>0){
                        //printf("\t");
                        *(content+ind)='\t';
                        ind++;
                        x--;
                    }
                    //printf("}\n");
                    *(content+ind)='}';
                        ind++;
                    if(*(field+i+1)!='\n'){
                        *(content+ind)='\n';
                        ind++;
                    }
                }
                else if(*(field+i-1)=='\t'){
                    //printf("}\n");
                    *(content+ind)='}';
                        ind++;
                    if(*(field+i+1)!='\n'){
                        *(content+ind)='\n';
                        ind++;
                    }
                    cnt--;
                    // int x=cnt;
                    // while(x>0){
                    //     //printf("\t");
                    //     *(content+ind)='\t';
                    //     ind++;
                    //     x--;
                    // }
                }
                else{
                    cnt--;
                    int x=cnt;
                    while(x>0){
                        //printf("\t");
                        *(content+ind)='\t';
                        ind++;
                        x--;
                    }
                    //printf("}\n");
                    *(content+ind)='}';
                        ind++;
                    if(*(field+i+1)!='\n'){
                        *(content+ind)='\n';
                        ind++;
                    }
                }
                } 
            }
            FILE*file=fopen(address,"w");
            fputs(content,file);
            fclose(file);
        }
}
void compare(char*address11,char*address21){
    char*address1=address11+1;
    char*address2=address21+1;
    if(check_exist(address1)){
        if(check_exist(address2)){
    char*first=file_to_string(address1);
    char*second=file_to_string(address2);
    int l1=1,l2=1;
    for(int i=0;*(first+i)!='\0';i++){
        if(*(first+i)=='\n'){
            l1++;
        }
    }
    for(int i=0;*(second+i)!='\0';i++){
        if(*(second+i)=='\n'){
            l2++;
        }
    }
    int l=(min(l1,l2));//tedade khat haye moshtarak
    FILE*file1=fopen(address1,"r");
    FILE*file2=fopen(address2,"r");
    for(int i=1;i<=l;i++){
        char line1[1000];
        char line2[1000];
        fgets(line1,1000,file1);
        fgets(line2,1000,file2);
        if(strcmp(line1,line2)){
            printf("============ #%d ============\n",i);
            printf("%s",line1);
            int k=strlen(line1);
            if(line1[k-1]!='\n')
            printf("\n");
            printf("%s",line2);
            int s=strlen(line2);
            if(line2[s-1]!='\n')
            printf("\n");
        }
    }
    if(l1>l2){
        printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",l2+1,l1);
        for(int i=l2+1;i<=l1;i++){
            char line1[1000];
            fgets(line1,1000,file1);
            printf("%s",line1);
            int k=strlen(line1);
            if(line1[k-1]!='\n')
            printf("\n");
        }
    }
    else if(l2>l1){
        printf(">>>>>>>>>>>> #%d - #%d >>>>>>>>>>>>\n",l1+1,l2);
        for(int i=l1+1;i<=l2;i++){
            char line2[1000];
            fgets(line2,1000,file2);
            printf("%s",line2);
            int s=strlen(line2);
            if(line2[s-1]!='\n')
            printf("\n");
        }
    }
    fclose(file1);fclose(file2);
}
}
}
void tree(const int num,char *root, int depth){
    int i;
    char *path=(char*)malloc(S*sizeof(char));
    struct dirent *dp;
    DIR *dir = opendir(root);
    if (!dir){
        return;
    }
    while ((dp = readdir(dir)) != NULL){
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0){
            for (i=0; i<num; i++){
                if (i%2 == 0 || i == 0){
                    printf("%c", 179);
                }
                else{
                    printf(" ");
                }
            }
            printf("%c%c%s\n", 195, 196, dp->d_name);
            if((depth==-1)||(depth>=num/2+1)){
                strcpy(path, root);
                strcat(path, "/");
                strcat(path, dp->d_name);
                tree( num + 2,path, depth);
            }
        }
    }
    closedir(dir);
}
void arman(){
    printf("Kill me!");
}
int main(){
    char*str=(char*)malloc(S*sizeof(char));
    char*command=(char*)malloc(100*sizeof(char));
    //char*address=(char*)malloc(1000*sizeof(char));
    //char*string=(char*)malloc(1000*sizeof(char));
    scanf("%[^\n]%*c", str);
    sscanf(str,"%s",command);
    
    if(strcmp(command,"createfile")==0){//working
        //do create file
        char*address=out(str,3);
        //printf("%s",address);
        createfile(address);
        
    }
    else if(strcmp(command,"insertstr")==0){//working
        //do insertstr
        char*address=out(str,3);
        char*string=out(str,5);
        char*pos=out(str,7);
        int line=give_line(pos);
        int index=give_index(pos);
        insertstr(address,string,line,index);
        
    }
    else if(strcmp(command,"cat")==0){//working
        //do cat
        char*address=out(str,3);
        cat(address);
    }
    else if(strcmp(command,"removestr")==0){//working
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
    else if(strcmp(command,"copystr")==0){//working
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
        //printf("%s",content_to_copy);
    }
    else if(strcmp(command,"cutstr")==0){//working
        //do cutstr
        char*address=out(str,3);
        char *pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        char*cize=out(str,7);
        int size=give_size(cize);
        char*direction=out(str,8);
        char direc=*(direction+1);
        //cutstr(address,line,index,size,direc);
        printf("%s",content_to_copy);
    }
    else if(strcmp(command,"pastestr")==0){//working
        //do pastestr
        char*address=out(str,3);
        char*pos=out(str,5);
        int line=give_line(pos);
        int index=give_index(pos);
        //printf("%s",content_to_copy);
        pastestr(address,line,index);
    }
    else if(strcmp(command,"find")==0){
        //do pastestr
        char*address=out(str,5);
        char*string=out(str,3);
        char*six=out(str,6);
        char*seven=out(str,7);
        char*eight=out(str,8);
        //printf("%s",string);
        if(strcmp(six,"-count")==0 && strcmp(seven,"ERROR")==0){
            find(string,address,2,seven);
        }
        if(strcmp(six,"-count")==0 && strcmp(seven,"-all")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"-count")==0 && strcmp(seven,"-at")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"-count")==0 && strcmp(seven,"-byword")==0){
            find(string,address,6,seven);
        }
        if(strcmp(six,"-at")==0 && strcmp(eight,"-all")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"-at")==0 && strcmp(eight,"-count")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"-at")==0 && strcmp(eight,"-byword")==0){
            find(string,address,5,seven);
        }
        else if(strcmp(six,"-at")==0 ){//at number
            find(string,address,1,seven);
        }
        if(strcmp(six,"-byword")==0 && strcmp(seven,"-count")==0){
            find(string,address,3,seven);
        }
        if(strcmp(six,"-byword")==0 && strcmp(seven,"-at")==0){
            find(string,address,5,eight);
        }
        if(strcmp(six,"-byword")==0 && strcmp(seven,"-all")==0){
            find(string,address,7,seven);
        }
        if(strcmp(six,"-byword")==0 && strcmp(seven,"ERROR")==0){
            find(string,address,3,seven);
        }
        if(strcmp(six,"-all")==0 && strcmp(seven,"ERROR")==0){
            find(string,address,4,seven);
        }
        if(strcmp(six,"-all")==0 && strcmp(seven,"-count")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"-all")==0 && strcmp(seven,"-byword")==0){
            find(string,address,7,seven);
        }
        if(strcmp(six,"-all")==0 && strcmp(seven,"-at")==0){
            printf("Invalid combination of attributes!");
        }
        if(strcmp(six,"ERROR")==0){
            find(string,address,1,seven);
        }
    }
    else if(strcmp(command,"replace")==0){
        //do replace
        char*address=out(str,7);
        char*string1=out(str,3);
        char*string2=out(str,5);
        char*mode=out(str,8);
        char*number=out(str,9);
        if(strcmp(mode,"ERROR")==0||strcmp(mode,"-at")==0){
            replace(string1,string2,address,1,number);
        }
        else{
            replace(string1,string2,address,2,number);
        }

    }
    else if(strcmp(command,"grep")==0){
        //do grep
        GREP_C=0;
        char*st=out(str,2);
        if(strcmp(st,"--str")==0){
            char*string=out(str,3);
            int i=5;
            while(strcmp(out(str,i),"ERROR")){
                char*address=out(str,i);
                    grep(string,address,1);
                i++;
            }
        }
        if(strcmp(st,"-C")==0){
            char*string=out(str,4);
            int i=6;
            while(strcmp(out(str,i),"ERROR")){
                char*address=out(str,i);
                    grep(string,address,2);
                i++;
            }
            printf("%d\n",GREP_C);
        }
        if(strcmp(st,"-I")==0){
            char*string=out(str,4);
            int i=6;
            while(strcmp(out(str,i),"ERROR")){
                char*address=out(str,i);
                    grep(string,address,3);
                i++;
            }
        }
    }
    else if(strcmp(command,"undo")==0){
        //do undo
        char*address=out(str,3);
        undo(address);
    }
    else if(strcmp(command,"auto-indent")==0){//working 
        
        char*address = out(str,2);
        //printf("%s",address);
        auto_indent(address);
    }
    else if(strcmp(command,"compare")==0){//working
        
        char*address1=out(str,2);
        char*address2=out(str,3);
        compare(address1,address2);

    }
    else if(strcmp(command,"tree")==0){
        char*dep=out(str,2);
        int depth=0;
        if(strcmp(dep,"-1")==0){
            depth=-1;
        }
        else{
            for(int i=0;*(dep+i)!='\0';i++){
            depth*=10;
            depth+=*(dep+i)-'0';
        }
        }
        tree(0,"./root",depth);
    }
    else{
        printf("Invalid command ");
    }
}













