#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

char str[1024];

void tail (int fd, char *name, int line) 
{
    int i, n;    
    int total=0; //menghitung total baris file
    int temp = open ("temp", O_CREATE | O_RDWR); //file menyimpan isi sementara
    while((n = read(fd, str, sizeof(str))) > 0 )
    {
        write (temp, str, n);                        
        for(i=0;i<n ;i++)
        {
            if(str[i]=='\n')            
            {
                total++; //menambah total baris jika ditemukan /n
            }
        }
    }
    close (temp);
    if (n < 0)
    {
        printf(1, "tail: read error\n");
        exit();
    }
    int fd1 = open("temp", 0); 
    int count=0;
    int start=total-line;                     
    while((n = read(fd1, str, sizeof(str))) > 0 )
    {
        for (i = 0; i<=n; i++) 
        {
            if (count >= start)         
            {
                printf(1,"%c",str[i]);
            }
            else if (str[i] == '\n')         
            {
                count++;
            }
        }
    }
    close (fd1); 
    unlink("temp");             
}


int main(int argc, char *argv[]) 
{
    int i;
    int fd = 0;	
    if (argc <= 1) 
    {	
        tail(0, "", 10);
        exit();
    }
    else 
    {
        char num[1024];
		strcpy(num,argv[1]);
		char *num_str=num;
		num_str=num_str+1;
		int x=atoi(num_str);
        if(argc==2)
        {
            if(num[0]=='-')
            {
			    tail(0,"",x);
			    close(fd);
            }
            else
            {
                if((fd=open(argv[1],0))<0)
			    {
				    printf(1,"tail:error opening the %s\n",argv[1]);
				    exit();
			    }
			    tail(fd,argv[1],10);
			    close(fd);
            }
        }
        else
        {
            if(num[0]=='-')
            {
                for(i=2;i<argc;i++)
		        {
			        if((fd=open(argv[i],0))<0)
			        {
				        printf(1,"tail:error opening the %s\n",argv[i]);
				        exit();
			        }
			        tail(fd,argv[i],x);
			        close(fd);
		        }
            }
            else
            {
                for(i=1;i<argc;i++)
		        {
			        if((fd=open(argv[i],0))<0)
			        {
				        printf(1,"tail:error opening the %s\n",argv[i]);
				        exit();
			        }
			        tail(fd,argv[i],10);
			        close(fd);
		        }
            }
           
        } 
    }
}
