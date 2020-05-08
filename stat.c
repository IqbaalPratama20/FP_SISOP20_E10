#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"

char *fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
	char *p;
	
    // untuk menemukan karakter pertama setelah /
	for(p = path + strlen(path); p >= path && *p != '/'; p--);
    p++;
	
	// Return blank-padded name
	if (strlen(p) >= DIRSIZ) return p;
	memmove (buf, p, strlen(p));
	memset (buf + strlen(p), ' ', DIRSIZ - strlen(p));
	return buf;
}
// Contoh: stat file.txt -> menampilkan informasi file, size, blocks, 
// IO Blocks, File type, Device, Inode, Links, Access, Uid, Gid, Context, Access, Modify, Change, Birth

void stat_print(char *path){
  char buf[512];
  int fd;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "stat: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "stat: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    break;
  }
  close(fd);
}

int main(int argc, char *argv[]){
    stat_print(argv[1]);
    exit();
}
