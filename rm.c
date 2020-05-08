#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

int count = 0;
void r(char *path);
void ast();
char *strcat(char *a, char *b){
  char *temp=a;
  while(*a) ++a;
  while(*b) *a++=*b++;
  *a=0;
  return temp;
}

char *fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
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
    ast();
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

void ast(){
  char buf[512], *p;
  int fd;
  char *path = ".";
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    unlink(path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_FILE) {
        unlink(buf);
      }else {
        printf(1,"rm: cannot remove '%s': Is a directory\n", buf);
      }
    }
    break;
  }
  close(fd);
} 
