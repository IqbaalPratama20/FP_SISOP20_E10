#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

void r(char *path);
void ast(char *path);
char *strcat(char *a, char *b){
  char *temp=a;
  while(*a) ++a;
  while(*b) *a++=*b++;
  *a=0;
  return temp;
}

int main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    // printf(2, "Usage: rm files...\n");
    printf(1, "Usage: rm [OPTIONS] [files] ... \n");
    printf(1, "-r     : delete folder and file recursively\n");
    printf(1, "*      : delete all file in current path\n");
    printf(1, "-r *   : delete all file and folder in current path\n");
    exit();
  }

  if(strcmp(argv[1], "-r") == 0){
    if(strcmp(argv[2], "*") == 0){
        r(argv[3]);
    }else{
      for(i=2; i<argc; i++){
        r(argv[i]);
        unlink(argv[1]);
      }
    }
  }else if(strcmp(argv[1], "*") == 0){
    for(i=2; i<argc; i++){
      ast(argv[i]);
    }
  }else{
    for(i=1; i<argc; i++){
      if(unlink(argv[i]) < 0){
        printf(2, "rm: %s failed to delete\n", argv[i]);
        break;
      }
    }
  }
  exit();
}

void r(char *path){
  char *buffer = (char *)malloc(1024*sizeof(char));
  int open_status;
  struct dirent de;
  struct stat st;
  int path_length = strlen(path);
  
  if(path[path_length-1] == '/'){
    path[path_length-1] = '\0';
  }
  if((open_status = open(path, 0)) < 0){
    printf(1,"rm: cannot open %s No such file or directory\n",path);
    exit();
  }
  if(fstat(open_status, &st)){
     printf(1,"rm: cannot stat %s No such file or directory\n",path);
      close(open_status);
      exit();
  }
  if(st.type == T_FILE){
    unlink(path);
  }else if(st.type == T_DIR){
    strcpy(buffer, path);
    strcat(buffer, "/");
    int length = strlen(buffer);
    while(read(open_status, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || de.name[0] == '.'){
        continue;
      }
      memmove(buffer+length, de.name, strlen(de.name));
      r(buffer);
      memset(buffer+length, '\0', sizeof(buffer)+length);
    }
    unlink(path);
  }
  free(buffer);
  close(open_status);
}

void ast(char *path){
  // char cwd[1024] = {0};
  
} 
