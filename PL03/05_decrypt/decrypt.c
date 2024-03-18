#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>



#define INFO_FILE "decrypt.txt"

typedef struct cipher {
    char *secret;
    char *word;
    struct cipher *next;
} cipher_t;


cipher_t * cipher_tail = NULL;
cipher_t * cipher_head = NULL;

void populate_cipher_t(FILE *);
cipher_t * add_item(char *);
void setup_cipher_data();
void decrypt_line(char *, int);
char * decrypt_token(char *);
void strip_line(char *);


int main(int argc, char * argv[]){

    FILE *cipherfp, *encryptedfp;

    if(argc != 2){
        perror("Need the decrypt file as a second parameter");
        exit(EXIT_FAILURE);
    }

    encryptedfp = fopen(argv[1], "r");
    
    if (!encryptedfp) {
        perror("File opening failed");
         exit(EXIT_FAILURE);
    }

    setup_cipher_data();

    char *buf = NULL;
    // char buf[1024];
    pid_t c_id=0;
    int cnt = 0;
    int read = 0;
    size_t len = 1024;

    pid_t parent = getpid();

    while((read = getline(&buf,&len,encryptedfp))!=-1) {

        // parent process
        if(parent == getpid()){
            strip_line(buf);
            cnt++;
            c_id = fork();
            if (c_id == 0) {
                decrypt_line(buf, cnt);
                //exit(EXIT_SUCCESS);
            } else {
               waitpid(c_id, NULL, 0);
      
            }
        } 
        
        if(cnt == 20)
            break;
    } 

    if(parent == getpid())
        printf("[%d] Translation complete. %d lines translated\n", getpid(), cnt);
    return EXIT_SUCCESS;
    
}

void decrypt_line(char *b, int l){

    char tmp[1024];
    char *curr_tkn;

    memset(tmp, 0, sizeof(tmp));

    curr_tkn = strtok(b, " `'.,-—\n\t\r");

    while(curr_tkn != NULL){
        strcat(tmp, decrypt_token(curr_tkn));
        tmp[strlen(tmp)] = ' ';
        curr_tkn = strtok(NULL, " `'.,-—\n\t\r");
    }

    printf("[%d] line %d: %s\n", getpid(), l, tmp);
    
}

char * decrypt_token(char *tkn){

    cipher_t *s;

    s = cipher_tail;

    while(s != NULL){
        if(strcmp(tkn, s->secret) == 0){
            return s->word;
        }
        s = s->next;
    }
    return tkn;
}


void setup_cipher_data(){

    FILE *cipherfp;

    cipherfp=fopen(INFO_FILE, "r");

    // Read the file secret file and populate the cipher list;
    if(cipherfp == NULL){
        perror("Failed to open file!");
        exit(EXIT_FAILURE);
    }else {
        populate_cipher_t(cipherfp);
    }

    cipher_t *p = cipher_tail;
    int count =0;
    while(p != NULL){
        p = p->next;
    }

}


void populate_cipher_t(FILE * fp){

    char buf[1024];

    while(fgets(buf, sizeof buf, fp)!= NULL){     
        strip_line(buf);
        add_item(buf);
    } 

}

cipher_t * add_item(char *b){

    cipher_t * i;

    // where we will store the string content
    char *str;

    str = (char*)malloc(strlen(b)*sizeof(char));

    strcpy(str, b);

    if (str[strlen(str)-1] == '\r'){
        str[strlen(str)-1] = '\0';
    }

    i = malloc(sizeof(cipher_t));
    i->word = strtok(str, " ");
    printf("i->word %s\n", i->word);
    i->secret = strtok(NULL, "\n");
    printf("i->secret %s\n", i->secret);

    i->next = cipher_tail;
    cipher_tail = i;

    return i;
}

void strip_line(char *l){

    size_t s = strlen(l);

    for(int i=s; i > s-4; i--){
        if(l[i] == '\n' || l[i]== '\r'){
            l[i] = '\0';
        }
    }
}