#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_FILENAME_LENGTH 1000
#define MAX_COMMIT_MESSAGE_LENGTH 2000
#define MAX_LINE_LENGTH 1000
#define MAX_MESSAGE_LENGTH 1000

void print_command(int argc, char * const argv[]);
int run_init(int argc, char * const argv[]);
int creat_configs(char *username, char *email);
int run_add(int argc, char * const argv[]);
int add_to_staging(char * filepath);
int run_commit(int argc, char * const argv[]);
int last_commit_ID();
int check_directory(char *filepath);
int commit_staged_file(int commit_ID, char* message);
int find_last_commit(char* filepath);

int run_init(int argc, char * const argv[]){
    char cwd[MAX_FILENAME_LENGTH];
    if(getcwd(cwd, sizeof(cwd)) == NULL ) return 1;
    printf("%s\n", cwd );

    char tmp_dir[MAX_FILENAME_LENGTH];
    bool exists = false;
    struct dirent * entry;
    do{
    DIR *dir = opendir(".");
    if(dir == NULL) return 1;

    while ((entry =readdir(dir)) != NULL){
        if(entry ->d_type == DT_DIR && strcmp(entry ->d_name, ".neogit")==0){
            exists = true;
        }
    }
    closedir(dir);
    printf("%s\n", tmp_dir);
    if(getcwd(tmp_dir, sizeof(tmp_dir))== NULL)
        return 1;
    if(strcmp(tmp_dir, "/") !=0){
       if( chdir("..") !=0)
       return 1;
    }
    }while (strcmp(tmp_dir, "/") !=0);
    if(chdir(cwd) != 0) return 1;
   
    if(exists){
        perror("neogit repo already exists");
    }else{
        if(mkdir(".neogit", 0755)!=0) return 1;
        creat_configs("kimia", "kimiaalv84@gmail.com");
    }
    return 0;
}

int run_add(int argc, char * const argv[]){
    if(argc<3){
        perror("invalid command");
        return 1;
    }
    char* filepath = argv[2];
    if(add_to_staging(filepath)!=0) return 1;
    return 0;
}


int creat_configs(char *username, char *email){
    FILE *file = fopen(".neogit/config", "w");
    if(file == NULL) return 1;
    fprintf(file, "username: %s\n", username);
    fprintf(file, "email: %s\n", email);

    fclose(file);
    file = fopen(".neogit/tracks", "w");
    fclose(file);
    file = fopen(".neogit/staging", "w");
    fclose(file);
    return 0;
}

int add_to_staging(char * filepath){
    FILE *file = fopen(".neogiit/staging", "r");
    if(file = NULL) return 1;
    char line[MAX_LINE_LENGTH];
    while(fgets(line, sizeof(line) -1 , file) != NULL){
        int len = strlen(line);
        if(len>0 && line[len -1] == '\n'){
            line[len-1] = '\0';
        }
        if(strcmp(line , filepath) == 0){
            return 0;
        }
    }
    fclose(file);
    file = fopen(".neogit/staging", "a");
    if(file == NULL) return 1;
    fprintf(file, "%s\n", filepath);
    fclose(file);
    return 0;
}


void print_command(int argc, char * const argv[]){
    for(int i=0; i<argc; i++){
        fprintf(stdout, "%s ", argv[i]);
    }
    fprintf(stdout, "\n");
}
int main(int argc, char *argv[]){
    if(argc<2){
        perror("please enter a valid command");
        return 1;
    }
    if(strcmp(argv[1], "init") == 0){
        run_init(argc, argv);
    }
    else if(strcmp(argv[1], "add") == 0){
        run_add(argc, argv);
    }
    
    return 0;
}