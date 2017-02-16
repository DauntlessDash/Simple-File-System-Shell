#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include "filesystem.h"


tree* createBST(){ //creates the file tree and creates the root node, the name of the root is "root", but this name is only used for debugging
    tree*t = (tree*)malloc(sizeof(tree));
    char * name="root";
    node * z=createnode(name, 1);
    t->root=z;
    return t;
}

node* createnode(char * name, int type){ // creates a node and then creates a childlist for the children of the node
    node*n = (node*)malloc(sizeof(node));
    n->type=type;
    n->parent=NULL;
    n->children=createlist();
    n->name=name;
    n->childnode=NULL; //points to the child that connects this node to the parent node
    return n;
}

child* createchild(node * newnode){ // creates a child and sets the input node as the node that the child will point to
    child*c = (child*)malloc(sizeof(child));
    c->next=NULL;
    c->prev=NULL;
    c->e=newnode;
    newnode->childnode=c; //sets pointer of node to point to the child that connects newnode to its parent
    return c;
}

list* createlist(){
    list*l = (list*)malloc(sizeof(list)); //creates the child list
    l->first==NULL;
    return l;
}

void mkdir(tree* t, node* cwd, char * path){ //calls the create function for a directory
    node * n;
    n=create(t, cwd, path, 1);
    if(n!=NULL){
    //printf("Made Directory: %s\n", n->name);
    }
}

void touch(tree* t, node* cwd, char * path){ //calls the create function for a file
    node * n;
    n=create(t, cwd, path, 0);
    if(n!=NULL){
    //printf("Made File: %s\n", n->name);
    }
}

node * follow(tree* t, node * cwd, char * path, int remy){ //determines whether the follow should begin at the cwd or root and starts the recursive follow
    node * n;
    node * z;
    
    if(path[0]=='/'){
        path++;
        n=t->root;
        z = recfollow(t, n, path, remy); //follow with root as input for paths beginning with '/'
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
    else{
        n=cwd;
        z = recfollow(t, n, path, remy); //follow with cwd as input for all other paths
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
}

node * recfollow(tree* t, node * n, char * path, int remy){ //recursivly follows the path inputed and returns the end node of the path
   //puts("start recfollow");
    int z=0;
    z++;
    
    if(path[0] == '\0'){
        return n;
    }
    node * newt=NULL;
    node * newnode;
    child * newchild;
    char * token;
    token = (char *) malloc (101);
    child * current;
    int i = 0;
    char * temp;
    
    while(path[i]!='/' && path[i]!='\0'){ //creates token from first segment of path
        
        token[i]=path[i];
        i++;
    }
    token[strlen(token)+1]='\0'; //add endline
    path=path+i+1; //removes token from path
    if(strcmp(".",token)==0){
        newt=n;
    }
    else if(strcmp("..",token)==0){ //recursivly goes up if '..'
        if(n->parent == NULL){
            newt=n;
        }
        else{
        newt=n->parent;
        }
    }
    else if(n->children->first == NULL){
        temp = recPWD(n);
        if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
        }
        else{
        printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
        return NULL;
        
    }
    else{
        current=n->children->first;
        while(current!=NULL){
           if(strcmp(current->e->name,token)==0){
               newt=current->e;
            }
        current=current->next;
        }
    }
    
    if(newt==NULL){
        temp = recPWD(n);
        if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
        }
        else{
        printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
        return NULL;
    }
    if(*path == '\0'){
        if(remy==0){
            if(newt->type==0){
            return NULL;
        }
        else{
            return newt;
        }
        }
        else if(remy==1){
            return newt;
        }
    }
    else{
        if(newt->type==0){
            temp = recPWD(n);
            if(temp[0]=='/' && temp[1]=='\0'){
            printf("Path Error: directory ’/%s’ does not exist.\n", token);
            }
            else{
            printf("Path Error: directory ’%s/%s’ does not exist.\n", temp, token);}
            return NULL;
        }
        else{
            return recfollow(t, newt, path, remy);
        }
    }
}

node * create(tree* t, node * cwd, char * path, int type){ //calls create and determines whether to begin reccreate at root or cwd
    node * n;
    node * z;
    
    if(path[0]=='/'){
        path++;
        n=t->root;
        z = reccreate(t, n, path, type); // start reccreate at root
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
    else{
        n=cwd;
        z = reccreate(t, n, path, type); // start reccreate at cwd
        if(z==NULL){
            return NULL;
        }
        else{
            return z;
        }
    }
}

node * reccreate(tree* t, node * n, char * path, int type){
    //puts("start reccreate");
    int z=0;
    z++;
    
    if(path[0] == '\0'){
        return n;
    }
    node * newt=NULL;
    node * newnode;
    child * newchild;
    char * token;
    token = (char *) malloc (101);
    child * current;
    int i = 0;
    char * error;
    
    while(path[i]!='/' && path[i]!='\0'){
        
        token[i]=path[i];
        i++;
    }
    token[strlen(token)+1]='\0';
    path=path+i+1;
    if(strcmp(".",token)==0){
        newt=n;
    }
    else if(strcmp("..",token)==0){
        if(n->parent == NULL){
            newt=n;
        }
        else{
        newt=n->parent;
        }
    }
    else if(n->children->first == NULL){
       
        if(*path == '\0'){
            newnode=createnode(token, type);
            newnode->parent=n;
            newchild=createchild(newnode);
            n->children->first=newchild;
            
            return newnode;
        }
        else{
            
            newnode=createnode(token, 1);
            newnode->parent=n;
            newchild=createchild(newnode);
            n->children->first=newchild;
            return reccreate(t, newnode, path, type);
        }
    }
    else{
        current=n->children->first;
        while(current!=NULL){
          
           if(strcmp(current->e->name,token)==0){
               newt=current->e;
               
            }
        current=current->next;
        }
            if(newt==NULL){
                current=n->children->first;
                while(current!=NULL){
                    if(strcmp(current->e->name, token)==0){
                        if(type==1){
                        printf("Make Dir Error: Cannot create directory.\n");}
                        if(type==0){
                        printf("Touch Error: Cannot create file.\n");}
                        return NULL;
                    }
                    else if(((compare(current->e->name, token)==-1)&&(!((current->e->type==1)&&(type==0)))) || ((current->e->type==0)&&(type==1))){
                        if(*path == '\0'){
                            
                        newnode=createnode(token, type);
                        newnode->parent=n;
                        newchild=createchild(newnode);
                        newchild->prev=current->prev;
                        if(current->prev!=NULL){
                            current->prev->next=newchild;
                        }
                        current->prev=newchild;
                        newchild->next=current;
                        if(newchild->prev==NULL){
                            n->children->first=newchild;
                        }
                        return newnode;
                        }
                        else{
                            newnode=createnode(token, 1);
                            newnode->parent=n;
                            newchild=createchild(newnode);
                            newchild->prev=current->prev;
                            current->prev=newchild;
                            newchild->next=current;
                            if(newchild->prev==NULL){
                                 n->children->first=newchild;
                            }
                            return reccreate(t, newnode, path, type);
                        }
                    }
                    else if(current->next==NULL){
                        if(*path == '\0'){
                        newnode=createnode(token, type);
                        newnode->parent=n;
                        newchild=createchild(newnode);
                        newchild->prev=current;
                        current->next=newchild;
                        return newnode;
                        }
                        else{
                            newnode=createnode(token, 1);
                            newchild=createchild(newnode);
                            newnode->parent=n;
                            newchild->prev=current;
                            current->next=newchild;
                            return reccreate(t, newnode, path, type);
                        }
                    }
                    current=current->next;
                }
            }
            
    }
    
     
    if(newt == NULL){
         printf("Make Dir Error: Cannot create directory.\n");
        return;
    }
    
    if(path[0]=='\0'){
        error = recPWD(newt);
        printf("Path Error: '%s' already exists and cannot be created.\n", error);
        if(type==1){
               printf("Make Dir Error: Cannot create directory.\n");
        }
        else if(type==0){
                printf("Touch Error: Cannot create file.\n");
            
        }
        return NULL;
    }
    else{
        if(newt->type==0){
            error = recPWD(newt);
            printf("Path Error: Cannot create sub-directory content. '%s' is a file.\n", error);
             if(type==1){
                 
                printf("Make Dir Error: Cannot create directory.\n");}
             if(type==0){
                printf("Touch Error: Cannot create file.\n");}
                return NULL;
        }
        else{
            
            return reccreate(t, newt, path, type);
        }
    } 
}

int compare(char * L, char * R){ // 1 === L has higher precedence, -1 == R has higher precedence, 0 == equal
//puts("compare start");
    while(*L != '\0' || *R != '\0'){
        if(order(*L)<order(*R)){
            return 1;
        }
        
        else if(order(*R)<order(*L)){
            return -1;
        }
        L++;
        R++;
    }
    if(*L=='\0' && *R=='\0'){
        return 0;
    }
    if(*L=='\0'){
        return 1;
    }
    if(*R=='\0'){
        return -1;
    }
    
}

int order(char c){
    //printf("%c", c);
    if(c=='\0'){
        return 0;
    }
    if(c=='.'){
        return 1;
    }
    if(c=='-'){
        return 2;
    }
    if(c=='_'){
        return 3;
    }
    if(c>='0' && c<='9'){
        return 3+(c-'0');
    }
    if(c>='A' && c<='Z'){
        return 13+(c-'A');
    }
    if(c>='a' && c<='z'){
        return 29+(c-'a');
    }
    else{
        return 0;
    }
}

void ls(node * cwd){
    char * path = recPWD(cwd);
    child * current = cwd->children->first;
    printf("Listing For ’%s’:\n", path);
    while(current!=NULL){
        if(current->e->type == 1)
        printf("D %s\n", current->e->name);
        else if(current->e->type == 0)
        printf("F %s\n", current->e->name);
        current=current->next;
    }
}

char * recPWD(node* n){
    char * path = NULL;
    node * current;
    current = n;
    char * slash = "/";
    int len;
    
    while(current->parent != NULL){
        if(path == NULL){
            path=current->name;
        }
        else{
            path = add(slash, path);
            path = add(current->name, path);
        }
        current=current->parent;
    }
    
    if(path == NULL){
            return slash;
        }
    else{
            return add(slash, path);
        }
}

char* add(const char* a, const char* b){
    size_t len = strlen(a) + strlen(b);
    char *newt = (char*)malloc(len * sizeof(char) + 1);
    *newt = '\0';
    return strcat(strcat(newt, a) ,b);
}

void rm(tree* t, node* cwd, char * path){
    node * n;
    char * temp;
    child * current;
    
    n=follow(t, cwd, path, 1);
    if(n==NULL){
    printf("Remove Error: Cannot remove file or directory.\n");
    return;
    }
    else{
        if(n->children->first!=NULL){
            temp = recPWD(n);
            printf("Remove Error: directory '%s' is not empty.\n", temp);
            return;
        }
        else{
           
            current=n->childnode;
            if(current->prev!=NULL){
            current->prev->next=current->next;
            }
            if(current->next!=NULL){
            current->next->prev=current->prev;
            }
            if(n->parent->children->first==current && current->next == NULL){
                n->parent->children->first=NULL;
            }
            else if(n->parent->children->first==current && current->next != NULL){
                n->parent->children->first=current->next;
            }
            free(n);
            free(current);
            
        }
    }
}

void rmf(tree* t, node* cwd, char * path){
    node * n;
    char * temp;
    child * current;
    
    n=follow(t, cwd, path, 1);
    if(n==NULL){
    printf("Remove Error: Cannot remove file or directory.\n");
    return;
    }
    else{
            current=n->childnode;
            if(current->prev!=NULL){
            current->prev->next=current->next;
            }
            if(current->next!=NULL){
            current->next->prev=current->prev;
            }
            if(n->parent->children->first==current && current->next == NULL){
                n->parent->children->first=NULL;
            }
            else if(n->parent->children->first==current && current->next != NULL){
                n->parent->children->first=current->next;
            }
            free(n);
            free(current);
    }
    
}

void find(tree* t, node* cwd, char * token){
    //printf("Searching For '%s':\n", token);
    recfind(t,t->root,token);
    return;
    
    
}

void recfind(tree* t, node* n, char * token){
    char * temp;
    char * location;
    child * current;
    
    temp = strstr(n->name, token); 
    if(temp!=NULL){ 
    if(n->parent!=NULL){
        location = recPWD(n);
        if(n->type==0){
            printf("F %s\n", location);
        }
        else if(n->type==1){
            printf("D %s\n", location);
        }
    }
    }
    if(n->children->first!=NULL){
    current=n->children->first;
    while(current!=NULL){
        recfind(t, current->e, token);
        current=current->next;
    }
    }
}