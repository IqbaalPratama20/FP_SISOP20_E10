#include "types.h"
#include "user.h"
#include "fcntl.h"
#include "stat.h"
#include "stdlib.h"


int main(int argc, char *argv[])
{
    int mode;
    if (argc != 3)
    {
        printf(1, "Invalid parameters for chmod. Parameter for chmod: [chmod] [mode] [file/directory]\n");
    }
    else 
    {
        mode = strtol(argv[1], NULL, 8); 
        if(chmod(argv[2], mode) == 0)
        {
            printf(1, "chmod was successful! Updating permission\n");
        }
        else
        {
            printf(1, "chmod command was unsuccessful\n");
        }
    }
    exit();
}

