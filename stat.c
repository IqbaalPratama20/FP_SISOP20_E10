#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
#include "date.h"

// this function calculate a power b
long long power(int x,int y)
{
	long long res = 1;
	for(int i=0;i<y;i++)
	{
		res = res * x;
	}
	return(res);
}

// this function whether the current year is a leap year
int check_leap(int x)
{
	int flag = 0;
	if(x%400==0)
		flag = 1;
	else if(x%100==0)
		flag = 0;
	else if(x%4==0)
		flag = 1;
	else 
		flag = 0;
	return(flag);
}

// this function prints the name of the month of the year
void month_name(int x)
{
	switch(x)
	{
		case 1:printf(1," Jan");
		       break;
		case 2:printf(1," Feb");
		       break;
		case 3:printf(1," Mar");
		       break;
		case 4:printf(1," Apr");
		       break;
		case 5:printf(1," May");
		       break;
		case 6:printf(1," Jun");
		       break;
		case 7:printf(1," Jul");
		       break;
		case 8:printf(1," Aug");
		       break;
		case 9:printf(1," Sep");
		       break;
		case 10:printf(1," Oct");
		       break;
		case 11:printf(1," Nov");
		       break;
		case 12:printf(1," Dec");
		       break;
	}
}

// this function prints the name of the day of the week
void day_name(int x,int y,int z)
{
	int initial_day = 4;
	int count = 0;	
	if(x>1970)
	{
		for(int i=1970;i<x;i++)
		{
			if(check_leap(i))
				count += 366;
			else
				count += 365;
		}
	}
	for(int i=1;i<y;i++)
	{
		if(i==2)
		{
			if(check_leap(x))
				count += 29;
			else
				count += 28;
		}
		else if((i<8)&&(i%2==1))
			count += 31;
		else if((i<8)&&(i%2==0))
			count += 30;
		else if((i>=8)&&(i%2==0))
			count += 31;
		else
			count += 30;
	}
	int final_day = (initial_day+count+z-1)%7;
	switch(final_day)
	{
		case 0:printf(1,"Sun");
		       break;
		case 1:printf(1,"Mon");
		       break;
		case 2:printf(1,"Tue");
		       break;
		case 3:printf(1,"Wed");
		       break;
		case 4:printf(1,"Thur");
		       break;
		case 5:printf(1,"Fri");
		       break;
		case 6:printf(1,"Sat");
		       break;
	 }
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
// Contoh: stat file.txt -> menampilkan informasi file, size, blocks, 
// IO Blocks, File type, Device, Inode, Links, Uid, Gid, Context, Access, Modify, Change, Birth

void stat_print(char *path){
  char buf[512], *p;
  int fd;
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

   if(date(st.st_atime)) {
    printf(2, "date failed\n");
    exit();
  }

  switch(st.type){
  case T_FILE:
    printf(1, 
      "name: %s |--| size: %d |--| blocks: %d |--| IO Blocks: %d \n", 
      fmtname(path), st.size, st.blksize, st.blocks);
    printf(1, 
      "type: File |--| Device: %d |--| Inode: %d |--| Links: %d \n", 
      st.dev, st.ino, st.nlink);
    unlink(path);
    printf(1, 
      "UID: %d |--| GID: %d |--| Context:  \n", 
      st.uid, st.gid);
    printf(1, 
      "Access time: %d-%d-%d %d:%d:%d\n", 
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Modify:  %d-%d-%d %d:%d:%d\n",
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Change:  %d-%d-%d %d:%d:%d\n",
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Birth:  \n" 
      );  
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    printf(1, 
      "name: %s |--| size: %d |--| blocks: %d |--| IO Blocks: %d \n", 
      fmtname(buf), st.size, st.blksize, st.blocks);
    printf(1, 
      "type: Directory |--| Device: %d |--| Inode: %d |--| Links: %d \n", 
      st.dev, st.ino, st.nlink);
    unlink(path);
    printf(1, 
      "UID: %d |--| GID: %d |--| Context:  \n", 
      st.uid, st.gid);
    printf(1, 
      "Access time: %d-%d-%d %d:%d:%d\n", 
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Modify:  %d-%d-%d %d:%d:%d\n",
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Change:  %d-%d-%d %d:%d:%d\n",
      st.st_atime->year, st.st_atime->month, st.st_atime->day, st.st_atime->hour, 
      st.st_atime->minute, st.st_atime->second);
    printf(1, 
      "Birth:  \n" 
      );
    break;

  case T_DEV:
  break;

  }
}

int main(int argc, char *argv[]){
    stat_print(argv[1]);
    exit();
}

